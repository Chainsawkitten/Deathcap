#pragma once

#include <cstdint>
#include <string>
#include "../linking.hpp"
#include "../Hymn.hpp"

namespace Audio {
    /// Interface for sound files of various formats.
    /**
     * Used to get raw audio data to a SoundBuffer.
     */
    class SoundFile {
        public:
            /// Destructor.
            ENGINE_API virtual ~SoundFile();
            
            /// Get raw audio data.
            /**
             * @param offset Number of samples from start of file.
             * @param samples Number of samples to read.
             * @param data Address to store raw audio data.
             * @return Number of valid samples loaded from file.
             */
            virtual int GetData(uint32_t offset, uint32_t samples, float*& data) const = 0;
            
            /// Get sample count.
            /**
             * @return The number of samples in the raw audio data.
             */
            virtual uint32_t GetSampleCount() const = 0;
            
            /// Get sample rate.
            /**
             * @return The sound file's sample rate (Hz).
             */
            virtual uint32_t GetSampleRate() const = 0;

            /// Get channel count.
            /**
             * @return The number of channels in audio.
             */
            virtual uint32_t GetChannelCount() const = 0;

            /// Whether sound file should be cached or streamed.
            /**
             * @param cache Whether to cache file.
             */
            virtual void Cache(bool cache) = 0;

            /// Check whether sound if loaded.
            /**
             * @return Whether sound is loaded or not.
             */
            virtual bool IsLoaded() const = 0;

            /// Whether sound file is cached or streamed.
            /**
             * @return Whether file is cached.
             */
            ENGINE_API bool GetCached() const;

            /// Save sound.
            ENGINE_API void Save() const;

            /// Load sound.
            /**
             * @param name Name of the sound file on disk.
             */
            ENGINE_API void Load(const std::string& name);

            /// The name of the sound.
            std::string name;

            /// The folder containing the sound file.
            std::string path;

        protected:
            /// Pointer to cached sound data.
            float* buffer = nullptr;

        private:
            virtual void Load(const char* filename) = 0;
    };
}
