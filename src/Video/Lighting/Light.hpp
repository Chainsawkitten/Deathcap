#pragma once

#include <glm/glm.hpp>

namespace Video {
    /// A light used for lighting the scene.
    struct Light {
        /// Position.
        glm::vec4 position;
        
        /// Intensity.
        glm::vec3 intensities;
        
        /// Attenuation.
        float attenuation;
        
        /// Ambient coefficient.
        float ambientCoefficient;
        
        /// Angle of the cone (in degrees).
        float coneAngle;
        
        /// Direction.
        glm::vec3 direction;
    };
}