cmake_minimum_required(VERSION 3.10)

if (ANDROID_ABI MATCHES "darwin")
    set(SOURCES src/AudioProcessor.cpp src/AudioProcessor.h src/main.cpp)
else()
    set(SOURCES src/AudioProcessor.cpp src/AudioProcessor.h)
endif()

# ANDROID
set(ANDROID_PLATFORM android-21)

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

project(bandmates-audio-processing)
if (ANDROID_ABI MATCHES "darwin")
    add_executable(bandmates-audio-processing ${SOURCES})
else()
    add_library(bandmates-audio-processing SHARED ${SOURCES})
endif()

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
