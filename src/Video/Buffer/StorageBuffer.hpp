#pragma once

#include <GL/glew.h>

namespace Video {
    /// Storage buffer used to store and retrieve data on the GPU.
    class StorageBuffer {
        public:
            /// Create new storage buffer.
            // TMP TODO
            StorageBuffer(int size, GLenum usage);
            
            /// Destructor.
            ~StorageBuffer();

            // TMP TODO
            void Write(void* data, int offset, int length);

            // TMP TODO
            void Bind();

            // TMP TODO
            void Unbind();

            // TMP TODO
            void BindBase(unsigned int binding);

            // TMP TODO
            int GetSize() const;

        private:
            StorageBuffer(const StorageBuffer & other) = delete;
            GLuint ssbo;
            int size;
            bool bound;
    };
}
