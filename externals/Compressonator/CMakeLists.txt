set(SRCS
        Codec_DXTC_RGBA.cpp
        CompressonatorXCodec.cpp
    )

add_library(Compressonator STATIC ${SRCS})
set_property(TARGET Compressonator PROPERTY CXX_STANDARD 11)
set_property(TARGET Compressonator PROPERTY CXX_STANDARD_REQUIRED ON)
