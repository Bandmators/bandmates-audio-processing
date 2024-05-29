#include <vector>

class AudioProcessor
{
public:
    std::vector<int16_t> getAudioPCM(char *audio, size_t size);
};