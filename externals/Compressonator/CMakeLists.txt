set(SRCS
        Codec_DXTC_Alpha.cpp
        Codec_DXTC_RGBA.cpp
        CompressonatorXCodec.cpp
    )

add_library(Compressonator STATIC ${SRCS})
set_property(TARGET Compressonator PROPERTY CXX_STANDARD 11)
set_property(TARGET Compressonator PROPERTY CXX_STANDARD_REQUIRED ON)

# Enable SSE2 when compiling 64-bit.
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse2 -msse3")
    target_compile_definitions(Compressonator PRIVATE USE_SSE=1)
endif()
