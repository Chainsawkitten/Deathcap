#pragma once

#include <GL/glew.h>

class World;
class Entity;
namespace Video {
    class Renderer;
    class Shader;
    class ShaderProgram;
}

/// Holds the frame buffers used for deferred rendering.
class DeferredLighting {
    public:
        /// The different buffer types.
        enum TEXTURE_TYPE {
            DIFFUSE, ///< Diffuse texture
            NORMAL, ///< Normals
            SPECULAR, ///< Specular
            GLOW, ///< Glow
            NUM_TEXTURES ///< Total number of textures (excluding depth buffer)
        };
        
        /// Create new render target.
        /**
         * @param renderer Renderer to use for rendering.
         */
        DeferredLighting(Video::Renderer* renderer);
        
        /// Destructor
        /**
         * Free allocated resources.
         */
        ~DeferredLighting();
        
        /// Reset render target.
        /**
         * Needs to be called before rendering to the screen.
         */
        void ResetTarget();
        
        /// Render the lighting in the world.
        /**
         * @param world World to get the lights from.
         * @param camera Camera to use.
         */
        void Render(World& world, const Entity* camera);
        
    private:
        static void AttachTexture(GLuint texture, unsigned int width, unsigned int height, GLenum attachment, GLint internalFormat);
        
        Video::Renderer* renderer;
        
        GLuint textures[NUM_TEXTURES];
        
        GLuint frameBufferObject;
        GLuint depthHandle;
        
        Video::Shader* vertexShader;
        Video::Shader* fragmentShader;
        Video::ShaderProgram* shaderProgram;
        
        // Store light uniform locations so we don't have to get them every frame.
        static const unsigned int lightCount = 32U;
        struct {
            GLint position;
            GLint intensities;
            GLint attenuation;
            GLint ambientCoefficient;
            GLint coneAngle;
            GLint direction;
        } lightUniforms[lightCount];
};
