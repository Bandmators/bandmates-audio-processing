#include "AudioProcessor.h"
#include <iostream>
#include <fstream>

char* readFileToCharArray(const std::string& filename, size_t& size) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
        return nullptr;
    }

    file.seekg(0, std::ios::end);
    size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buffer = new char[size];

    file.read(buffer, size);

    file.close();

    return buffer;
}

int main()
{
    AudioProcessor* audioProcessor = new AudioProcessor();
    size_t size = 0;
    auto byteArray = readFileToCharArray("sample.mp3", size);
    auto pcm = audioProcessor->getAudioPCM(byteArray, size);
    int i = 0;
    for (auto it = pcm.begin(); i < 5000; it++)
    {
        std::cout << (*it) << std::endl;
        i++;
    }
    return 0;
}