#pragma once

namespace Video {
    class RenderTarget;
    class Filter;
    namespace Geometry {
        class Rectangle;
    }
}

/// Applies post-processing effects to the rendered image.
class PostProcessing {
    public:
        /// Create new post-processing handler.
        PostProcessing();
        
        /// Destructor.
        ~PostProcessing();
        
        /// Get render target.
        /**
         * @return Target to render to.
         */
        Video::RenderTarget* GetRenderTarget() const;

        /// Updates the buffer size.
        void UpdateBufferSize();
        
        /// Apply a filter to the render image.
        /**
         * filter %Filter to apply.
         */
        void ApplyFilter(Video::Filter* filter);
        
        /// Render resulting image to screen.
        /**
         * @param dither Whether to use dithering.
         */
        void Render(bool dither = false);
        
    private:
        short int which = 0;
        
        Video::RenderTarget* buffers[2];
        
        Video::Geometry::Rectangle* rectangle;
};
