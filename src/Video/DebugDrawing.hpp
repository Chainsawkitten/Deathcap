#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "linking.hpp"

namespace Video {
    class ShaderProgram;
    
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
                /// The dimensions of the cuboid.
                glm::vec3 dimensions;
                
                /// The matrix used to transform the cuboid.
                glm::mat4 matrix;
                
                /// Color.
                glm::vec3 color;
                
                /// Line width.
                float lineWidth;
                
                /// Duration (in seconds).
                float duration;
                
                /// Whether to enable depth testing.
                bool depthTesting;
            };
            
            /// A debug drawing plane.
            struct Plane {
                /// The center position of the plane.
                glm::vec3 position;
                
                /// The plane normal.
                glm::vec3 normal;
                
                /// Size.
                glm::vec2 size;
                
                /// Color.
                glm::vec3 color;
                
                /// Line width.
                float lineWidth;
                
                /// Duration (in seconds).
                float duration;
                
                /// Whether to enable depth testing.
                bool depthTesting;
            };
            
            /// A debug drawing circle.
            struct Circle {
                /// The center position of the circle.
                glm::vec3 position;
                
                /// The circle normal.
                glm::vec3 normal;
                
                /// Radius.
                float radius;
                
                /// Color.
                glm::vec3 color;
                
                /// Line width.
                float lineWidth;
                
                /// Duration (in seconds).
                float duration;
                
                /// Whether to enable depth testing.
                bool depthTesting;
            };
            
            /// A debug drawing sphere.
            struct Sphere {
                /// The center position of the sphere.
                glm::vec3 position;
                
                /// Radius.
                float radius;
                
                /// Color.
                glm::vec3 color;
                
                /// Line width.
                float lineWidth;
                
                /// Duration (in seconds).
                float duration;
                
                /// Whether to enable depth testing.
                bool depthTesting;
            };
            
            /// A debug drawing cylinder.
            struct Cylinder {
                /// Radius.
                float radius;
                
                /// Length.
                float length;
                
                /// The matrix used to transform the cylinder.
                glm::mat4 matrix;
                
                /// Color.
                glm::vec3 color;
                
                /// Line width.
                float lineWidth;
                
                /// Duration (in seconds).
                float duration;
                
                /// Whether to enable depth testing.
                bool depthTesting;
            };
            
            /// A debug drawing cone.
            struct Cone {
                /// Radius.
                float radius;
                
                /// Height.
                float height;
                
                /// The matrix used to transform the cone.
                glm::mat4 matrix;
                
                /// Color.
                glm::vec3 color;
                
                /// Line width.
                float lineWidth;
                
                /// Duration (in seconds).
                float duration;
                
                /// Whether to enable depth testing.
                bool depthTesting;
            };
            
            /// Create new debug primitive renderer.
            VIDEO_API DebugDrawing();
            
            /// Destructor.
            VIDEO_API ~DebugDrawing();
            
            /// Start rendering debug primitives.
            /**
             * Needs to be called before DrawPoint, DrawLine or DrawCuboid.
             * @param viewProjectionMatrix The camera's view projection matrix.
             */
            VIDEO_API void StartDebugDrawing(const glm::mat4& viewProjectionMatrix);
            
            /// Draw a point.
            /**
             * @param point The point to draw.
             */
            VIDEO_API void DrawPoint(const Point& point);
            
            /// Draw a line.
            /**
             * @param line The line to draw.
             */
            VIDEO_API void DrawLine(const Line& line);
            
            /// Draw a cuboid.
            /**
             * @param cuboid The cuboid to draw.
             */
            VIDEO_API void DrawCuboid(const Cuboid& cuboid);
            
            /// Draw a plane.
            /**
             * @param plane The plane to draw.
             */
            VIDEO_API void DrawPlane(const Plane& plane);
            
            /// Draw a circle.
            /**
             * @param circle The circle to draw.
             */
            VIDEO_API void DrawCircle(const Circle& circle);
            
            /// Draw a sphere.
            /**
             * @param sphere The sphere to draw.
             */
            VIDEO_API void DrawSphere(const Sphere& sphere);
            
            /// Draw a cylinder.
            /**
             * @param cylinder The cylinder to draw.
             */
            VIDEO_API void DrawCylinder(const Cylinder& cylinder);
            
            /// Draw a cone.
            /**
             * @param cone The cone to draw.
             */
            VIDEO_API void DrawCone(const Cone& cone);
            
            /// Stop debug drawing.
            VIDEO_API void EndDebugDrawing();
            
        private:
            DebugDrawing(const DebugDrawing & other) = delete;
            
            static void CreateVertexArray(const glm::vec3* positions, unsigned int positionCount, GLuint& vertexBuffer, GLuint& vertexArray);
            void BindVertexArray(GLuint vertexArray);
            void CreateCircle(glm::vec3*& positions, unsigned int& vertexCount, unsigned int detail);
            void CreateSphere(glm::vec3*& positions, unsigned int& vertexCount, unsigned int detail);
            void CreateCylinder(glm::vec3*& positions, unsigned int& vertexCount, unsigned int detail);
            void CreateCone(glm::vec3*& positions, unsigned int& vertexCount, unsigned int detail);
            
            Video::ShaderProgram* shaderProgram;
            
            // Uniform locations.
            GLuint viewProjectionLocation;
            GLuint modelLocation;
            GLuint colorLocation;
            GLuint sizeLocation;
            
            // Geometry.
            GLuint pointVertexBuffer;
            GLuint pointVertexArray;
            
            GLuint lineVertexBuffer;
            GLuint lineVertexArray;
            
            GLuint cuboidVertexBuffer;
            GLuint cuboidVertexArray;
            
            GLuint planeVertexBuffer;
            GLuint planeVertexArray;
            
            GLuint sphereVertexBuffer;
            GLuint sphereVertexArray;
            unsigned int sphereVertexCount;
            
            GLuint circleVertexBuffer;
            GLuint circleVertexArray;
            unsigned int circleVertexCount;
            
            GLuint cylinderVertexBuffer;
            GLuint cylinderVertexArray;
            unsigned int cylinderVertexCount;
            
            GLuint coneVertexBuffer;
            GLuint coneVertexArray;
            unsigned int coneVertexCount;
            
            GLuint boundVertexArray = 0;
    };
}
