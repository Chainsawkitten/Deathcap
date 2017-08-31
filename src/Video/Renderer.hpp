#pragma once

namespace Video {
    /// Handles rendering using OpenGL.
    class Renderer {
        public:
            /// Create new renderer.
            Renderer();
            
            /// Destructor.
            ~Renderer();
            
            /// Clear the previous frame's data.
            void Clear();
    };
}
