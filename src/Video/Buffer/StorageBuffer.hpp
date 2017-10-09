#pragma once

#include <GL/glew.h>

namespace Video {
    /// Storage buffer used to store and retrieve data on the GPU.
    class StorageBuffer {
        public:
            /// Create new storage buffer.
            // TMP TODO
            StorageBuffer(unsigned int size, GLenum usage);
            
            /// Destructor.
            ~StorageBuffer();

            // TMP TODO
            void Write(void* data, unsigned int offset, unsigned int length);

            // TMP TODO
            void Bind();

            // TMP TODO
            void Unbind();

            // TMP TODO
            void BindBase(unsigned int binding);

            // TMP TODO
            unsigned int GetSize() const;

        private:
            StorageBuffer(const StorageBuffer & other) = delete;
            GLuint ssbo;
            int size;
            bool bound;
    };
}
