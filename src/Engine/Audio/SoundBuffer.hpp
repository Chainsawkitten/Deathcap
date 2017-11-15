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
            friend class SoundStreamer;
            /// Constructor.
            //TMPTODO
            ENGINE_API SoundBuffer(unsigned int samplesPerChunk = 512, unsigned int chunkCount = 5);
            
            /// Destructor.
            ENGINE_API ~SoundBuffer();
            
            //TMPTODO
            /// Get sound data.
            /**
             * @return The sound file containing the sound.
             */
            ENGINE_API int GetData(float* data, unsigned int samples);

            // TMPTODO
            ENGINE_API SoundFile* GetSoundFile() const;

            //TMPTODO
            /// Get AL buffer.
            /**
             * @param soundFile The sound file containing the sound.
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
            unsigned int bufferSampleCount;

            unsigned int samplesPerChunk;
            unsigned int chunkCount;

            unsigned int begin = 0;
            unsigned int end = 0;
    };
}
