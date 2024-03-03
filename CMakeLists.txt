cmake_minimum_required(VERSION 3.10)

project(bandmates-audio-processing)

set(SOURCES src/main.cpp)
add_executable(bandmates-audio-processing ${SOURCES})

# FFMPEG
set(ARCH_ABI darwin)
set(FFMPEG ${CMAKE_SOURCE_DIR}/third-party/ffmpeg/${ARCH_ABI})

file(GLOB FFMPEG_LIB ${FFMPEG}/lib/*.*)
target_include_directories(bandmates-audio-processing PRIVATE ${FFMPEG}/include)
target_link_libraries(bandmates-audio-processing ${FFMPEG_LIB})

add_custom_command(
    TARGET bandmates-audio-processing
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${ARCH_ABI}/lib
    COMMAND ${CMAKE_COMMAND} -E copy ${FFMPEG_LIB} ${CMAKE_BINARY_DIR}/${ARCH_ABI}/lib
)