#pragma once

#include <glm/glm.hpp>

namespace Video {
    class ShaderProgram;
    class RenderSurface;
    class RenderTarget;
    class Filter;
    namespace Geometry {
        class Rectangle;
    }
    
    /// Applies post-processing effects to the rendered image.
    class PostProcessing {
        public:
            /// Create new post-processing handler.
            /**
             * @param screenSize The size of the screen in pixels.
             * @param rectangle %Rectangle to use for rendering.
             */
            PostProcessing(const glm::vec2& screenSize, const Geometry::Rectangle* rectangle);
            
            /// Destructor.
            ~PostProcessing();
            
            /// Get render target.
            /**
             * @return Target to render to.
             */
            RenderTarget* GetRenderTarget() const;
    
            /// Updates the buffer size.
            /**
             * @param screenSize The size of the screen in pixels.
             */
            void UpdateBufferSize(const glm::vec2& screenSize);
            
            /// Apply a filter to the render image.
            /**
             * TODO
             * filter %Filter to apply.
             */
            void ApplyFilter(Video::RenderSurface* renderSurface, Filter* filter);
            
            /// Render resulting image to screen.
            /**
             * TODO
             * @param dither Whether to use dithering.
             */
            void Render(Video::RenderSurface* renderSurface, bool dither = false);
            
        private:
            short int which = 0;
            RenderTarget* buffers[2];
            const Geometry::Rectangle* rectangle;

            float ditherTime = 0.f;

            ShaderProgram* shaderProgram;
            ShaderProgram* ditherShaderProgram;
    };
}
