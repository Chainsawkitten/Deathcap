#pragma once

#include <glm/glm.hpp>

namespace Video {
    /// Draws debug primitives.
    class DebugDrawing {
        public:
            /// A debug drawing point.
            struct Point {
                /// World position.
                glm::vec3 position;
                
                /// Color.
                glm::vec3 color;
                
                /// Size.
                float size;
                
                /// Duration (in seconds).
                float duration;
                
                /// Whether to enable depth testing.
                bool depthTesting;
            };
            
            /// A debug drawing line.
            struct Line {
                /// Starting position of the line.
                glm::vec3 startPosition;
                
                /// End position of the line.
                glm::vec3 endPosition;
                
                /// Color.
                glm::vec3 color;
                
                /// Line width.
                float width;
                
                /// Duration (in seconds).
                float duration;
                
                /// Whether to enable depth testing.
                bool depthTesting;
            };
            
            /// A debug drawing cuboid.
            struct Cuboid {
                /// The minimum coordinates of the box.
                glm::vec3 minCoordinates;
                
                /// The maximum coordinates of the box.
                glm::vec3 maxCoordinates;
                
                /// Color.
                glm::vec3 color;
                
                /// Line width.
                float lineWidth;
                
                /// Duration (in seconds).
                float duration;
                
                /// Whether to enable depth testing.
                bool depthTesting;
            };
    };
}
