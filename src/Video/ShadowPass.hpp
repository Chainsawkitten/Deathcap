#pragma once

#include "linking.hpp"

namespace Video {
    class ShadowPass
    {
    public:
        VIDEO_API ShadowPass();
        VIDEO_API ~ShadowPass();

        ////Create shadowbuffer
        /**
        *
        */
        VIDEO_API void InitDephtMap();

        ////Bind Texture to depth buffer
        /**
        *
        */
        VIDEO_API void BindBuffer();

        VIDEO_API int GetShadowWidth();
        VIDEO_API int GetShadowHeight();
        VIDEO_API int GetDepthMapFbo();
        VIDEO_API int GetShadowID();


    private:
        unsigned int depthMap;
        unsigned int depthMapFbo;
        const unsigned int SHADOW_WIDTH = 1024;
        const unsigned int SHADOW_HEIGHT = 1024;
        
    };
}

