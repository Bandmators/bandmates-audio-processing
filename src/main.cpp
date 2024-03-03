#include <iostream>

extern "C"
{
    #include <libavformat/avformat.h>
}

int main(int argc, char *argv[])
{
    const char *audio_file = "sample.mp3";

    AVFormatContext *format_ctx = nullptr;

    if (avformat_open_input(&format_ctx, audio_file, nullptr, nullptr) != 0)
    {
        std::cout << "success" << std::endl;
    }
    else 
    {
        std::cout << "failed" << std::endl;
    }

    return 0;
}