if [ $1 == "MAC" ] ; then
    cmake -DANDROID_ABI=darwin -Bbuild/darwin
    cmake --build build/darwin
    cp -r third-party/ffmpeg/darwin build
fi
if [ $1 == "ANDROID" ] ; then
    ABIs=(arm64-v8a armeabi-v7a x86 x86_64)
    for ABI in ${ABIs[@]}
    do
        cmake -DANDROID_ABI=$ABI -DCMAKE_TOOLCHAIN_FILE=/Users/gimdoyeob/Library/Android/sdk/ndk/25.1.8937393/build/cmake/android.toolchain.cmake -Bbuild/$ABI
        cmake --build build/$ABI
        cp build/$ABI/libbandmates-audio-processing.so /Users/gimdoyeob/AndroidStudioProjects/bandmates-android/app/src/main/cpp/libs/$ABI
    done
    cp src/AudioProcessor.h /Users/gimdoyeob/AndroidStudioProjects/bandmates-android/app/src/main/cpp/include
fi
