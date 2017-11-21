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
            
            /// Get raw audio data.
            /**
             * @param offset Number of samples from start of file.
             * @param samples Number of samples to read.
             * @param data Address to store raw audio data.
             * @return Number of valid samples loaded from file.
             */
            ENGINE_API int GetData(uint32_t offset, uint32_t samples, float*& data) const final;
            
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

            /// Whether sound file should be cached or streamed.
            /**
             * @param cache Whether to cache file.
             */
            ENGINE_API void Cache(bool cache) final;
            
        private:
            void Load(const char* filename);

            stb_vorbis* stbFile = nullptr;
            int sampleCount = 0;
            int channelCount = 0;
            int sampleRate = 0;
    };
}
