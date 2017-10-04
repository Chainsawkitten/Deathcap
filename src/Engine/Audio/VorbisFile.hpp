#pragma once

#include "SoundFile.hpp"

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
            VorbisFile(const char* filename);
            
            /// Destructor.
            ~VorbisFile() final;
            
            /// Get raw audio data.
            /**
             * @return Raw audio data.
             */
            char* GetData() const final;
            
            /// Get data size.
            /**
             * @return The length of the raw audio data.
             */
            size_t GetSize() const final;
            
            /// Get sample rate.
            /**
             * @return The sound file's sample rate (Hz).
             */
            size_t GetSampleRate() const final;
            
        private:
            char* data;
            int dataSize;
            int sampleRate;
            ALenum format;
    };
}
