#pragma once

#include "../linking.hpp"
#include <thread>
#include <queue>
#include <vector>
#include <mutex>

namespace Audio {

    class SoundFile;

    /// Streams sound data from file.
    class SoundStreamer {
        public:
            /// Constructor.
            //TMPTODO
            ENGINE_API SoundStreamer();
            
            /// Destructor.
            ENGINE_API ~SoundStreamer();

            struct DataHandle {
                public:
                    SoundFile* soundFile = nullptr;
                    uint32_t offset = 0;
                    uint32_t samples = 0;
                    float* data = nullptr;
                    bool done = false;
            };

            // TMPTODP
            ENGINE_API void Load(SoundStreamer::DataHandle& dataHandle);

            // TMPTODO
            bool stopWorker = false;
           
        private:
            class Worker {
            public:
                void Start(SoundStreamer* soundStreamer);
                void Execute(SoundStreamer* soundStreamer);
                void Join();

            private:
                std::thread workThread;
            } worker;

            std::queue<DataHandle*> loadQueue;

            std::mutex mutex;
    };
}
