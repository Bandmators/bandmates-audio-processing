#include <iostream>
#include <fstream>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

#include "AudioProcessor.h"

using namespace std;

int custom_read_packet(void *opaque, uint8_t *buf, int buf_size)
{
    istream *file = static_cast<istream *>(opaque);

    file->read(reinterpret_cast<char *>(buf), buf_size);
    int bytes_read = file->gcount();

    if (file->eof())
        return bytes_read;
    else if (file->bad())
        return AVERROR(errno);

    return bytes_read;
}

class AudioStreamBuffer : public streambuf
{
public:
    AudioStreamBuffer(char *source, size_t size)
    {
        setg(source, source, source + size);
    }

    virtual ~AudioStreamBuffer()
    {
    }
};

class AudioInputStream : public istream
{
public:
    AudioInputStream(char *source, size_t size) : istream(&buffer), buffer(source, size)
    {
    }

    virtual ~AudioInputStream()
    {
    }

private:
    AudioStreamBuffer buffer;
};

vector<int16_t> AudioProcessor::getAudioPCM(char *audio, size_t size)
{
    std::vector<int16_t> pcmData;

    AVFormatContext *format_ctx = avformat_alloc_context();
    if (format_ctx == nullptr)
    {
        std::cout << "context alloc failed\n";
        return pcmData;
    }

    const int packetSize = 4096;
    uint8_t *io_buffer = (uint8_t *)av_malloc(packetSize);
    cout << size << endl;
    AudioInputStream audioInputStream(audio, size);
    AVIOContext *io_ctx = avio_alloc_context(io_buffer, packetSize, 0, static_cast<void *>(&audioInputStream), &custom_read_packet, NULL, NULL);
    if (io_ctx == nullptr)
    {
        std::cout << "io context alloc failed\n";
        return pcmData;
    }

    format_ctx->pb = io_ctx;

    if (avformat_open_input(&format_ctx, NULL, nullptr, nullptr) != 0)
    {
        std::cout << "open failed\n";
        return pcmData;
    }

    if (avformat_find_stream_info(format_ctx, nullptr) < 0)
    {
        std::cout << "find stream info failed\n";
        avformat_close_input(&format_ctx);
        return pcmData;
    }

    const AVCodec *codec = avcodec_find_decoder(format_ctx->streams[0]->codecpar->codec_id);
    if (codec == nullptr)
    {
        std::cout << "find decoder failed\n";
        avformat_close_input(&format_ctx);
        return pcmData;
    }

    AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
    if (codec_ctx == nullptr)
    {
        std::cout << "codec alloc context failed\n";
        avformat_close_input(&format_ctx);
        return pcmData;
    }

    codec_ctx->sample_fmt = AV_SAMPLE_FMT_S16;

    if (avcodec_open2(codec_ctx, codec, NULL) < 0)
    {
        std::cout << "codec open failed\n";
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&format_ctx);
        return pcmData;
    }

    AVPacket *packet = av_packet_alloc();
    packet->data = NULL;
    packet->size = 0;

    AVFrame *frame = av_frame_alloc();
    if (!frame)
    {
        std::cout << "frame alloc failed\n";
        avformat_close_input(&format_ctx);
        return pcmData;
    }

    while (av_read_frame(format_ctx, packet) >= 0)
    {
        if (packet->stream_index != 0)
            continue;
        if (avcodec_send_packet(codec_ctx, packet) != 0)
            continue;
        while (avcodec_receive_frame(codec_ctx, frame) == 0)
        {
            for (int i = 0; i < frame->nb_samples; ++i)
            {
                pcmData.push_back(((int16_t)frame->data[0][2 * i] << 8) | frame->data[0][2 * i + 1]);
                pcmData.push_back(((int16_t)frame->data[1][2 * i] << 8) | frame->data[1][2 * i + 1]);
            }
        }
        av_packet_unref(packet);
    }

    avformat_close_input(&format_ctx);
    avcodec_free_context(&codec_ctx);
    av_frame_free(&frame);

    return pcmData;
}
