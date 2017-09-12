#pragma once

#include <glm/glm.hpp>
#include "FrameBuffer.hpp"
#include "ReadWriteTexture.hpp"

namespace Video {
    
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
            
            /// Get deferred rendering frame buffer.
            /**
             * @return Deferred rendering frame buffer.
             */
            FrameBuffer* GetDeferredFrameBuffer() const;

            /// Get size of the render target.
            /**
             * @return The size of the render target in pixels.
             */
            glm::vec2 GetSize() const;
            
        private:
            glm::vec2 size;
            
            // Deferred textures.
            ReadWriteTexture* albedoTexture;
            ReadWriteTexture* normalTexture;
            ReadWriteTexture* specTexture;
            ReadWriteTexture* glowTexture;
            ReadWriteTexture* depthTexture;

            // Frame buffers.
            FrameBuffer* deferredFrameBuffer;
    };
}
