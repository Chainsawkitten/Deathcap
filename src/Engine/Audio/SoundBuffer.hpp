#pragma once

#include <AL/al.h>
#include <string>
#include <json/json.h>

namespace Audio {
    class SoundFile;
    
    /// Wrapper for OpenAL buffers.
    class SoundBuffer {
        public:
            /// Create new unloaded sound buffer.
            SoundBuffer();
            
            /// Create a sound buffer from a sound file.
            /**
             * @param soundFile The sound file containing the sound.
             */
            SoundBuffer(SoundFile* soundFile);
            
            /// Destructor.
            ~SoundBuffer();
            
            /// Get AL buffer.
            /**
             * @return The OpenAL buffer ID.
             */
            float* GetBuffer() const;
            
            /// Save the sound.
            /**
             * @return JSON value to be stored on disk.
             */
            Json::Value Save() const;
            
            /// Load sound from file.
            /**
             * @param name Name of the sound to load.
             */
            void Load(const std::string& name);
            
            /// Load sound buffer from a sound file.
            /**
             * @param soundFile The sound file containing the sound.
             */
            void Load(SoundFile* soundFile);

            /// Get size of buffer.
            /**
             * @return size_t The size of the buffer.
             */
            size_t GetSize();

            /// The name of the sound.
            std::string name;
            
            /// The folder containing the sound file.
            std::string path;
            
        private:
            float* buffer;
            size_t size;
            size_t sampleRate;
    };
}
