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
            /// Get sound data.
            /**
             * @return The OpenAL buffer ID.
             */
            ENGINE_API int GetData(float* data, unsigned int samples);

            // TMPTODO
            ENGINE_API SoundFile* GetSoundFile() const;

            //TMPTODO
            /// Get AL buffer.
            /**
             * @return The OpenAL buffer ID.
             */
            ENGINE_API void SetSoundFile(SoundFile* soundFile);

            //TMPTODO
            /// Get AL buffer.
            /**
             * @return The OpenAL buffer ID.
             */
            ENGINE_API void Restart();
            
        private:
            SoundFile* soundFile = nullptr;
            float* buffer = nullptr;
            int bufferSampleCount;

            int beginSample = 0;
            int endSample = 0;

            int currentSample = 0;
    };
}
