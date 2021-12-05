cd icu4c-70.1/source
chmod +x runConfigureICU configure install-sh \
    && ./runConfigureICU MacOSX \
    && make -j4
