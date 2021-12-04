# build glew macos arm64
cd glew-2.2.0/build
cmake -DCMAKE_APPLE_SILICON_PROCESSOR=arm64 ./cmake
make -j4
