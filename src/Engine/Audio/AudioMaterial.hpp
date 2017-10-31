#pragma once

#include <string>
#include <json/json.h>
#include "../linking.hpp"

namespace Audio {
    /// Contains information about how sound should interact with the material.
    class AudioMaterial {
        public:
            /// Create new audio material.
            ENGINE_API AudioMaterial();

            /// Destructor.
            ENGINE_API ~AudioMaterial();

            /// Saves the audio material to Json value.
            /**
             * @return The saved data in a JSON node.
             */
            ENGINE_API Json::Value Save() const;

            /// Loads the audio material information.
            /**
             * @param name Name of the script.
             */
            ENGINE_API void Load(const std::string& name);

            /// The name of the sound.
            std::string name;

            /// The folder containing the sound file.
            std::string path;

            /// Fraction of sound energy absorbed at low frequencies. 
            /// Between 0.0 and 1.0.
            float lowFreqAbsorption = 0.5f;

            /// Fraction of sound energy absorbed at middle frequencies. 
            /// Between 0.0 and 1.0.
            float midFreqAbsorption = 0.5f;

            /// Fraction of sound energy absorbed at high frequencies.  
            /// Between 0.0 and 1.0.
            float highFreqAbsorption = 0.5f;

            /// Fraction of sound energy that is scattered in a random direction when it reaches the surface.
            /// Between 0.0 and 1.0. 
            /// A value of 0.0 describes a smooth surface with mirror - like reflection properties; a value of 1.0 describes rough surface with diffuse reflection properties.
            float scattering = 0.5f;

            /// Fraction of sound energy transmitted through at low frequencies. 
            /// Between 0.0 and 1.0.
            float lowFreqTransmission = 0.5f;

            /// Fraction of sound energy transmitted through at middle frequencies.
            /// Between 0.0 and 1.0.
            float midFreqTransmission = 0.5f;

            /// Fraction of sound energy transmitted through at high frequencies. 
            /// Between 0.0 and 1.0.
            float highFreqTransmission = 0.5f;
    };
}
