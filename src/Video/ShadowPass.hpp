#pragma once

#include "linking.hpp"

namespace Video {
    class ShadowPass {
        public:
            ///Create new default shadow program.
            VIDEO_API ShadowPass();
            VIDEO_API ~ShadowPass();

            ///Create shadow buffer for writing
            VIDEO_API void InitDepthMap();

            ///Bind Texture to depth buffer
            VIDEO_API void BindBuffer();

            ///return shadowmaps width
            /**
             * @return shadowmaps width
             */
            VIDEO_API int GetShadowWidth();

            ///return shadowmaps height
            /**
             * @return shadowmaps height
             */
            VIDEO_API int GetShadowHeight();

            ///return shadowmap framebuffer object
            /**
             * @return shadowmap framebuffer object
             */
            VIDEO_API int GetDepthMapFbo();

            ///return shadowmap texture id
            /**
             * @return shadowmap texture id
             */
            VIDEO_API int GetShadowID();

        private:
            unsigned int depthMap;
            unsigned int depthMapFbo;
            const unsigned int SHADOW_WIDTH = 1024;
            const unsigned int SHADOW_HEIGHT = 1024;
    };
} // namespace Video
