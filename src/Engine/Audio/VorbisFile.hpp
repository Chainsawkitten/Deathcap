#pragma once

#include "SoundFile.hpp"
#include "../linking.hpp"

namespace Audio {
    /// Ogg Vorbis implementation of SoundFile.
    /**
     * Loads .ogg-files.
     */
    class VorbisFile : public SoundFile {
        public:
            /// Loads the specified ogg-file.
            /**
             * @param filename Filename (relative or absolute) to ogg-file.
             */
            ENGINE_EXPORT VorbisFile(const char* filename);
            
            /// Destructor.
            ENGINE_EXPORT ~VorbisFile() final;
            
            /// Get raw audio data.
            /**
             * @return Raw audio data.
             */
            ENGINE_EXPORT float* GetData() const final;
            
            /// Get data size.
            /**
             * @return The length of the raw audio data.
             */
            ENGINE_EXPORT uint32_t GetSize() const final;
            
            /// Get sample rate.
            /**
             * @return The sound file's sample rate (Hz).
             */
            ENGINE_EXPORT uint32_t GetSampleRate() const final;
            
        private:
            float* data;
            int dataSize;
            int sampleRate;
    };
}
