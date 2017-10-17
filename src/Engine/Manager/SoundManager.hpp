#pragma once

#include "../Entity/ComponentContainer.hpp"
#include <portaudio.h>
#include "../Audio/SteamAudioInterface.hpp"
#include "../linking.hpp"

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
         * @param err The PortAudio error number to check.
         */
        ENGINE_EXPORT static void CheckError(PaError err);
        
        /// Moves sound sources and plays sounds.
        /**
         * @param deltaTime Time since last frame.
         */
        ENGINE_EXPORT void Update(float deltaTime);
        
        /// Create sound source component.
        /**
         * @return The created component.
         */
        ENGINE_EXPORT Component::SoundSource* CreateSoundSource();
        
        /// Create sound source component.
        /**
         * @param node Json node to load the component from.
         * @return The created component.
         */
        ENGINE_EXPORT Component::SoundSource* CreateSoundSource(const Json::Value& node);
        
        /// Get all sound source components.
        /**
         * @return All sound source components.
         */
        ENGINE_EXPORT const std::vector<Component::SoundSource*>& GetSoundSources() const;
        
        /// Create listener component.
        /**
         * @return The created component.
         */
        ENGINE_EXPORT Component::Listener* CreateListener();
        
        /// Create listener component.
        /**
         * @param node Json node to load the component from.
         * @return The created component.
         */
        ENGINE_EXPORT Component::Listener* CreateListener(const Json::Value& node);
        
        /// Get all listener components.
        /**
         * @return All listener components.
         */
        ENGINE_EXPORT const std::vector<Component::Listener*>& GetListeners() const;
        
        /// Remove all killed components.
        ENGINE_EXPORT void ClearKilledComponents();
        
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
