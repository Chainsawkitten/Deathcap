#pragma once

#include <glm/glm.hpp>
namespace Video {
    
    class FrameBuffer;
    class ReadWriteTexture;

    /// Render surface containing framebuffers.
    class RenderSurface {
        public:
            /// Create new render surface.
            /**
             * @param size Size of the new render surface.
             */
            RenderSurface(const glm::vec2& size);
            
            /// Destructor.
            ~RenderSurface();
            
            /// Get size of the render target.
            /**
            * @return The size of the render target in pixels.
            */
            glm::vec2 GetSize() const;

            /// Get deferred rendering frame buffer.
            /**
             * @return Deferred rendering frame buffer.
             */
            FrameBuffer* GetDeferredFrameBuffer() const;

            /// Get texture contaning albedo pixel data.
            /**
             * @return The albedo texture.
             */
            ReadWriteTexture* GetAlbedoTexture() const;

            /// Get texture contaning normal pixel data.
            /**
             * @return The normal texture.
             */
            ReadWriteTexture* GetNormalTexture() const;

            /// Get texture contaning specular pixel data.
            /**
             * @return The specular texture.
             */
            ReadWriteTexture* GetSpecularTexture() const;

            /// Get texture contaning glow pixel data.
            /**
             * @return The glow texture.
             */
            ReadWriteTexture* GetGlowTexture() const;

            /// Get texture contaning depth pixel data.
            /**
             * @return The depth texture.
             */
            ReadWriteTexture* GetDepthTexture() const;

            // Post processing textures. TODO
            ReadWriteTexture* colorTexture[2];
            ReadWriteTexture* extraTexture[2];
            ReadWriteTexture* depth2Texture[2];

            FrameBuffer* postProcessingFrameBuffer[2];
            
        private:
            glm::vec2 size;

            // Frame buffers.
            FrameBuffer* deferredFrameBuffer;

            // Deferred textures.
            ReadWriteTexture* albedoTexture;
            ReadWriteTexture* normalTexture;
            ReadWriteTexture* specTexture;
            ReadWriteTexture* glowTexture;
            ReadWriteTexture* depthTexture;
    };
}
