#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Light.hpp"
#include <vector>

namespace Video {
    class FrameBuffer;
    class RenderSurface;
    class ShaderProgram;
    class StorageBuffer;
    namespace Geometry {
        class Rectangle;
    }
    
    /// Holds the light buffer.
    class Lighting {
        public:
            /// Create new Forward lighting.
            /**
             * @param shaderProgram %Shader used for rendering. 
             */
            Lighting(ShaderProgram* shaderProgram);
            
            /// Destructor.
            ~Lighting();
            
            /// Clear all lights.
            void ClearLights();
            
            /// Add a light.
            /**
             * @param light The light to add.
             */
            void AddLight(const Light& light);
            
            /// Light the scene with the added lights.
            /**
             * @param inverseProjectionMatrix The camera's inverse projection matrix.
             * @param renderSurface %RenderSurface contaning textures.
             */
            void Render(const glm::mat4& inverseProjectionMatrix, RenderSurface* renderSurface);
            
        private:
            Lighting(const Lighting & other) = delete;
            const Geometry::Rectangle* rectangle;
            ShaderProgram* shaderProgram;
            
            std::vector<Light> lights;
            StorageBuffer* lightBuffer;
            
            // Store light uniform locations so we don't have to get them every frame.
            static const unsigned int lightCount = 32U;
            struct {
                GLint position;
                GLint intensities;
                GLint attenuation;
                GLint ambientCoefficient;
                GLint coneAngle;
                GLint direction;
                GLint color;
            } lightUniforms[lightCount];
    };
}
