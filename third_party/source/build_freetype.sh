cd freetype-2.11.1
rm -rf build
cmake -B build \
    -D CMAKE_BUILD_TYPE=Release \
    -D BUILD_SHARED_LIBS=true \
    -D CMAKE_APPLE_SILICON_PROCESSOR=arm64 \
    && cmake --build build
