#pragma once

#include "SuperComponent.hpp"
#include <cstdint>
#include "../linking.hpp"

class SoundManager;
namespace Audio {
    class SoundBuffer;
}

namespace Component {
    /// %Component describing a sound source.
    class SoundSource : public SuperComponent {
        friend class ::SoundManager;
        
        public:
            /// Create new sound source.
            ENGINE_API SoundSource();
            
            /// Destructor.
            ENGINE_API ~SoundSource();
            
            /// Save the component.
            /**
             * @return JSON value to be stored on disk.
             */
            ENGINE_API Json::Value Save() const override;
            
            /// Play the sound.
            ENGINE_API void Play();
            
            /// Pause the sound.
            ENGINE_API void Pause();
            
            /// Stop the sound.
            ENGINE_API void Stop();
            
            /// Sound buffer.
            Audio::SoundBuffer* soundBuffer;
            
            /// Volume.
            float volume = 1.f;
            
            /// Whether the sound should loop.
            bool loop = false;
            
        private:            
            bool shouldPlay = false;
            bool shouldPause = false;
            bool shouldStop = false;
    };
}
