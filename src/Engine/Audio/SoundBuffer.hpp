#pragma once

#include <string>
#include <json/json.h>
#include <cstdint>
#include "../linking.hpp"

namespace Audio {
    class SoundFile;
    
    /// Wrapper for OpenAL buffers.
    class SoundBuffer {
        public:
            /// Create new unloaded sound buffer.
            ENGINE_EXPORT SoundBuffer();
            
            /// Create a sound buffer from a sound file.
            /**
             * @param soundFile The sound file containing the sound.
             */
            ENGINE_EXPORT SoundBuffer(SoundFile* soundFile);
            
            /// Destructor.
            ENGINE_EXPORT ~SoundBuffer();
            
            /// Get AL buffer.
            /**
             * @return The OpenAL buffer ID.
             */
            ENGINE_EXPORT float* GetBuffer() const;
            
            /// Save the sound.
            /**
             * @return JSON value to be stored on disk.
             */
            ENGINE_EXPORT Json::Value Save() const;
            
            /// Load sound from file.
            /**
             * @param name Name of the sound to load.
             */
            ENGINE_EXPORT void Load(const std::string& name);
            
            /// Load sound buffer from a sound file.
            /**
             * @param soundFile The sound file containing the sound.
             */
            ENGINE_EXPORT void Load(SoundFile* soundFile);

            /// Get size of buffer.
            /**
             * @return The size of the buffer.
             */
            ENGINE_EXPORT uint32_t GetSize();

            /// The name of the sound.
            std::string name;
            
            /// The folder containing the sound file.
            std::string path;
            
        private:
            float* buffer;
            uint32_t size;
            uint32_t sampleRate;
    };
}
