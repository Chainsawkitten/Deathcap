#pragma once

#include "SoundFile.hpp"
#include "../linking.hpp"

struct stb_vorbis;

namespace Audio {
    /// Ogg Vorbis implementation of SoundFile.
    /**
     * Loads .ogg-files.
     */
    class VorbisFile : public SoundFile {
        public:
            /// Constructor.
            ENGINE_API VorbisFile();

            /// Destructor.
            ENGINE_API ~VorbisFile() final;
            
            // TMPTODO
            /// Get raw audio data.
            /**
             * @return Raw audio data.
             */
            ENGINE_API int GetData(uint32_t offset, uint32_t samples, float* data) const final;
            
            /// Get sample count.
            /**
             * @return The number of samples in the raw audio data.
             */
            ENGINE_API uint32_t GetSampleCount() const final;
            
            /// Get sample rate.
            /**
             * @return The sound file's sample rate (Hz).
             */
            ENGINE_API uint32_t GetSampleRate() const final;

            /// Get channel count.
            /**
             * @return The number of channels in audio.
             */
            ENGINE_API uint32_t GetChannelCount() const final;
            
        private:

            /// Loads the specified ogg-file.
            /**
             * @param filename Filename (relative or absolute) to ogg-file.
             */
            ENGINE_API void Load(const char* filename);

            stb_vorbis* stbFile = nullptr;
            int sampleCount = 0;
            int channelCount = 0;
            int sampleRate = 0;
    };
}
