#pragma once

#include "../linking.hpp"
#include <thread>
#include <queue>
#include <vector>
#include <mutex>

namespace Audio {

    const unsigned int CHUNK_COUNT = 10U;
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
                public:
                    friend class Worker;
                    friend class SoundBuffer;
                    friend class SoundStreamer;

                    /// Constructor.
                    /**
                     * @param soundFile %SoundFile to stream read data.
                     * @param offset Number of samples from start of file to read from.
                     * @param samples Number of samples to read.
                     * @param data Address to store data from file.
                     */
                    DataHandle(SoundFile* soundFile, uint32_t offset, uint32_t samples, float* data);

                private:
                    SoundFile* soundFile = nullptr;
                    uint32_t offset = 0;
                    uint32_t samples = 0;
                    float* data = nullptr;
                    bool done = false;
                    bool abort = false;
            };

            /// Add work to streaming thread.
            /**
             * @param dataHandle %DataHandle containing information about the work that should be done.
             */
            ENGINE_API void Load(SoundStreamer::DataHandle& dataHandle);

            /// Remove all work for this queue in the streaming thread.
            /**
             * @param queue Queue to be flushed.
             */
            ENGINE_API void Flush(std::queue<DataHandle>& queue);
           
        private:
            Worker worker;

            std::queue<DataHandle*> loadQueue;

            std::mutex queueMutex;
            std::mutex flushMutex;

            bool stopWorker = false;
    };
}
