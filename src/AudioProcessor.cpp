#include "AudioProcessor.h"

using namespace std;

vector<int16_t> AudioProcessor::getAudioPCM(const char* audio_file)
{
    std::vector<int16_t> pcmData;

    AVFormatContext *format_ctx = nullptr;

    if (avformat_open_input(&format_ctx, audio_file, nullptr, nullptr) != 0)
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
