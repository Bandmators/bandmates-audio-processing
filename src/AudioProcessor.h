#include <iostream>
#include <vector>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

using namespace std;

class AudioProcessor
{
public:
    vector<int16_t> getAudioPCM(const char *audio_file);
};