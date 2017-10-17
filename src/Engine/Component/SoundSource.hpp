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
            ENGINE_EXPORT SoundSource();
            
            /// Destructor.
            ENGINE_EXPORT ~SoundSource();
            
            /// Save the component.
            /**
             * @return JSON value to be stored on disk.
             */
            ENGINE_EXPORT Json::Value Save() const override;
            
            /// Play the sound.
            ENGINE_EXPORT void Play();
            
            /// Pause the sound.
            ENGINE_EXPORT void Pause();
            
            /// Stop the sound.
            ENGINE_EXPORT void Stop();
            
            /// Sound buffer.
            Audio::SoundBuffer* soundBuffer = nullptr;
            
            /// Pitch.
            float pitch = 1.f;
            
            /// Gain.
            float gain = 1.f;
            
            /// Whether the sound should loop.
            bool loop = false;

            /// Place in buffer
            uint32_t place = 0;
            
        private:            
            bool shouldPlay = true;
            bool shouldPause = false;
            bool shouldStop = false;
            
            bool soundBufferSet = false;
    };
}
