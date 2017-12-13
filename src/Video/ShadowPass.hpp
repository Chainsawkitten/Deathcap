#pragma once

#include "linking.hpp"
///Handles writing and creation of shadowmap.
namespace Video {
    class ShadowPass {
        public:
            ///Create new default shadow program.
            VIDEO_API ShadowPass();

            ///Removes GL buffers.
            VIDEO_API ~ShadowPass();

            ///Create shadow buffer for writing.
            VIDEO_API void InitDepthMap();

            ///Bind Texture to depth buffer.
            VIDEO_API void BindBuffer();

            /// Get the size of the shadow map.
            /**
             * @return The size of the shadow map.
             */
            VIDEO_API unsigned int GetShadowMapSize() const;

            ///return shadowmap framebuffer object
            /**
             * @return shadowmap framebuffer object
             */
            VIDEO_API int GetDepthMapFbo();

            ///return shadowmap textures id
            /**
             * @return shadowmap textures id
             */
            VIDEO_API int GetShadowID();

        private:
            ///depthmap to save the texture.
            unsigned int depthMap;

            ///Depthmap framebuffer object.
            unsigned int depthMapFbo;

            /// Size of the shadowmap.
            unsigned int shadowSize = 1024;
    };
} // namespace Video
