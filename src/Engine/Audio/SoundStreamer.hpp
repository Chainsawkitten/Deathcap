#pragma once

#include "../linking.hpp"
#include <thread>
#include <mutex>
#include <Utility/Queue.hpp>

namespace Audio {

    const unsigned int CHUNK_COUNT = 3U;
    class SoundFile;

    /// Streams sound data from file.
    class SoundStreamer {
        public:
            /// Constructor.
            ENGINE_API SoundStreamer();
            
            /// Destructor.
            ENGINE_API ~SoundStreamer();

            /// Worker class used to wrapp threaded sound streaming.
            class Worker {
                public:
                    /// Create thread.
                    /**
                     * @param soundStreamer The Soundstreamer to get work from.
                     */
                    void Start(SoundStreamer* soundStreamer);

                    /// Start thread.
                    /**
                     * @param soundStreamer The Soundstreamer to get work from.
                     */
                    void Execute(SoundStreamer* soundStreamer);

                    /// Finalize thread.
                    void Join();

                private:
                    std::thread workThread;
            };

            /// Data handle to assign work to streaming thread, and check whether the work is done.
            struct DataHandle {
                /// Constructor.
                DataHandle() = default;

                /// Constructor.
                /**
                 * @param soundFile SoundFile to stream read data.
                 * @param offset Number of samples from start of file to read from.
                 * @param samples Number of samples to read.
                 * @param data Address to store data from file.
                 */
                DataHandle(SoundFile* soundFile, uint32_t offset, uint32_t samples, float* data);

                /// SoundFile to read from.
                SoundFile* soundFile = nullptr;

                /// Number of samples from start of file to read.
                uint32_t offset = 0;

                /// Number of samples to read.
                uint32_t samples = 0;

                /// Address to store data from file.
                float* data = nullptr;

                /// Whether handle is done.
                bool done = false;

                /// Whether handle is aborted.
                bool abort = false;
            };

            /// Add work to streaming thread.
            /**
             * @param handle DataHandle containing information about the work that should be done.
             */
            ENGINE_API void Load(SoundStreamer::DataHandle* handle);
           
        private:
            Worker worker;

            Utility::Queue<DataHandle*> loadQueue;

            std::mutex queueMutex;

            bool stopWorker = false;
    };
}
