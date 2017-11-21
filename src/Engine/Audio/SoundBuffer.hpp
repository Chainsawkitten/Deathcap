#pragma once

#include <string>
#include <json/json.h>
#include <cstdint>
#include "../linking.hpp"
#include <queue>
#include "SoundStreamer.hpp"
#include "../Manager/SoundManager.hpp"

namespace Audio {
    class SoundFile;
    
    /// Wrapper for sound file.
    class SoundBuffer {
        public:
            friend class SoundStreamer;

            /// Constructor.
            ENGINE_API SoundBuffer();
            
            /// Destructor.
            ENGINE_API ~SoundBuffer();
            
            /// Get sound data.
            /**
             * @param [OUT] Number of valid samples fetched from file.
             * @return Pointer to raw audio data.
             */
            ENGINE_API float* GetChunkData(int& samples);

            /// Release used sound data from stream.
            ENGINE_API void ConsumeChunk();

            /// Request new sound data to stream.
            ENGINE_API void ProduceChunk();

            /// Get sound file.
            /**
             * @return The %SoundFile containing the sound.
             */
            ENGINE_API SoundFile* GetSoundFile() const;

            /// Set sound file.
            /**
             * @param soundFile The %SoundFile containing the sound.
             */
            ENGINE_API void SetSoundFile(SoundFile* soundFile);

            /// Reset file.
            ENGINE_API void Restart();
            
        private:
            SoundFile* soundFile = nullptr;
            float* buffer = nullptr;

            unsigned int chunkCount = 0;
            unsigned int begin = 0;

            std::queue<SoundStreamer::DataHandle> chunkQueue;
    };
}
