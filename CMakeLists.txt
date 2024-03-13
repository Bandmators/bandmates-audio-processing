cmake_minimum_required(VERSION 3.10)

set(SOURCES src/AudioProcessor.cpp src/AudioProcessor.h)

# ANDROID
set(ANDROID_ABI x86_64)
set(ANDROID_PLATFROM android-21)

# FFMPEG
set(FFMPEG_INCLUDE ${CMAKE_SOURCE_DIR}/third-party/ffmpeg/${ANDROID_ABI}/include)
set(FFMPEG_LIB ${CMAKE_SOURCE_DIR}/third-party/ffmpeg/${ANDROID_ABI}/lib)

find_library(AVCODEC avcodec PATHS ${FFMPEG_LIB})
find_library(AVDEVICE avdevice PATHS ${FFMPEG_LIB})
find_library(AVFILTER avfilter PATHS ${FFMPEG_LIB})
find_library(AVFORMAT avformat PATHS ${FFMPEG_LIB})
find_library(AVUTIL avutil PATHS ${FFMPEG_LIB})
find_library(SWRESAMPLE swresample PATHS ${FFMPEG_LIB})
find_library(SWSCALE swscale PATHS ${FFMPEG_LIB})

set(CMAKE_TOOLCHAIN_FILE /Users/gimdoyeob/Library/Android/sdk/ndk/25.1.8937393/build/cmake/android.toolchain.cmake)
project(bandmates-audio-processing)
add_library(bandmates-audio-processing SHARED ${SOURCES})

target_link_libraries(
    bandmates-audio-processing PRIVATE 
    ${AVCODEC} 
    ${AVDEVICE} 
    ${AVFILTER} 
    ${AVFORMAT} 
    ${AVUTIL} 
    ${SWRESAMPLE} 
    ${SWSCALE} 
)

target_include_directories(bandmates-audio-processing PRIVATE ${FFMPEG_INCLUDE})
