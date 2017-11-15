#pragma once

#include "../linking.hpp"
#include <thread>
#include <mutex>

class SoundManager;

namespace Audio {
    class SoundBuffer;
    
    /// Streams sound data from file.
    class SoundStreamer {
        public:
            /// Constructor.
            //TMPTODO
            ENGINE_API SoundStreamer(SoundManager* soundManager);
            
            /// Destructor.
            ENGINE_API ~SoundStreamer();
           
        private:
            void Run(SoundManager* soundManager);

            std::thread thread;
            std::mutex mutex;
            std::unique_lock<std::mutex> lock;
            bool done = false;
    };
}
