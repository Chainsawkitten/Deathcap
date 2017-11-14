#pragma once

#include <string>
#include <json/json.h>
#include <cstdint>
#include "../linking.hpp"

namespace Audio {
    class SoundFile;
    
    /// Wrapper for sound file.
    class SoundBuffer {
        public:
            /// Constructor.
            ENGINE_API SoundBuffer();

            /// Create a sound buffer from a sound file.
            /**
             * @param soundFile The sound file containing the sound.
             */
            ENGINE_API explicit SoundBuffer(SoundFile* soundFile);
            
            /// Destructor.
            ENGINE_API ~SoundBuffer();
            
            //TMPTODO
            /// Get AL buffer.
            /**
             * @return The OpenAL buffer ID.
             */
            ENGINE_API void GetBuffer(float* data, uint32_t& samples);

            // TMPTODO
            ENGINE_API SoundFile* GetSoundFile() const;

            //TMPTODO
            /// Get AL buffer.
            /**
             * @return The OpenAL buffer ID.
             */
            ENGINE_API void SetSoundFile(SoundFile* soundFile);
            
        private:
            SoundFile* soundFile = nullptr;
            float* buffer = nullptr;
            uint32_t bufferStartSample = 0;
            uint32_t bufferSampleCount;
            uint32_t currentSample = 0;
    };
}
