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
            //TMPTODO
            ENGINE_API SoundBuffer();
            
            /// Destructor.
            ENGINE_API ~SoundBuffer();
            
            //TMPTODO
            /// Get sound data.
            /**
             * @return The sound file containing the sound.
             */
            ENGINE_API float* GetChunkData(int& samples);

            //TMPTODO
            ENGINE_API void ConsumeChunk();

            //TMPTODO
            ENGINE_API void ProduceChunk();

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
            float buffer[CHUNK_SIZE * CHUNK_COUNT];

            unsigned int begin = 0;

            std::queue<SoundStreamer::DataHandle> chunkQueue;
    };
}
