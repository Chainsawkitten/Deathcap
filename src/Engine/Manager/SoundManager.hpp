#pragma once

#include "../Entity/ComponentContainer.hpp"
#include "portaudio.h"
#include "../Audio/SteamAudioInterface.hpp"

namespace Component {
    class SoundSource;
    class Listener;
}
namespace Json {
    class Value;
}

/// Handles OpenAL sound.
class SoundManager {
    friend class Hub;
    
    public:     
        /// Check for OpenAL errors.
        /**
         * @param message Message to print to standard error if an error was encountered.
         */
        static void CheckError(PaError err);
        
        /// Moves sound sources and plays sounds.
        void Update(float deltaTime);
        
        /// Create sound source component.
        /**
         * @return The created component.
         */
        Component::SoundSource* CreateSoundSource();
        
        /// Create sound source component.
        /**
         * @param node Json node to load the component from.
         * @return The created component.
         */
        Component::SoundSource* CreateSoundSource(const Json::Value& node);
        
        /// Get all sound source components.
        /**
         * @return All sound source components.
         */
        const std::vector<Component::SoundSource*>& GetSoundSources() const;
        
        /// Create listener component.
        /**
         * @return The created component.
         */
        Component::Listener* CreateListener();
        
        /// Create listener component.
        /**
         * @param node Json node to load the component from.
         * @return The created component.
         */
        Component::Listener* CreateListener(const Json::Value& node);
        
        /// Get all listener components.
        /**
         * @return All listener components.
         */
        const std::vector<Component::Listener*>& GetListeners() const;
        
        /// Remove all killed components.
        void ClearKilledComponents();
        
    private:
        SoundManager();
        ~SoundManager();
        SoundManager(SoundManager const&) = delete;
        void operator=(SoundManager const&) = delete;
        
        SteamAudioInterface sAudio;
        PaStream* stream;
        float* processedFrameSamples;
        
        float volume = 1.f;
        
        ComponentContainer<Component::SoundSource> soundSources;
        ComponentContainer<Component::Listener> listeners;
};
