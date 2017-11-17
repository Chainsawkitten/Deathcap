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

            // TMPTODO
            class Worker {
            public:
                void Start(SoundStreamer* soundStreamer);
                void Execute(SoundStreamer* soundStreamer);
                void Join();

            private:
                std::thread workThread;
            };

            // TMPTODO
            struct DataHandle {
                public:
                    friend class Worker;
                    friend class SoundBuffer;
                    DataHandle(SoundFile* soundFile, uint32_t offset, uint32_t samples, float* data);

                    bool Done() const;

                private:
                    SoundFile* soundFile = nullptr;
                    uint32_t offset = 0;
                    uint32_t samples = 0;
                    float* data = nullptr;
                    bool done = false;
            };

            // TMPTODP
            ENGINE_API void Load(SoundStreamer::DataHandle& dataHandle);
           
        private:
            Worker worker;

            std::queue<DataHandle*> loadQueue;

            std::mutex mutex;

            bool stopWorker = false;
    };
}
