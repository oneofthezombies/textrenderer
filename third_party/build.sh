# build glew macos arm64
cd source/glew-2.2.0/build
cmake -DCMAKE_APPLE_SILICON_PROCESSOR=arm64 ./cmake
make -j4

# build glfw macos arm64
cd source/glfw-3.3.5
cmake -DCMAKE_APPLE_SILICON_PROCESSOR=arm64 -DBUILD_SHARED_LIBS=ON -Bbuild .
cd build
make -j4
