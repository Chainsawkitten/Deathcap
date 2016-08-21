#pragma once

#include <AL/al.h>
#include <string>

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
            ALuint Buffer() const;
            
            /// Load sound buffer from a sound file.
            /**
             * @param soundFile The sound file containing the sound.
             */
            void Load(SoundFile* soundFile);
            
            /// The name of the sound.
            std::string name;
            
        private:
            ALuint buffer;
    };
}
