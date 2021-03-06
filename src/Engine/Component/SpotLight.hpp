#pragma once

#include "SuperComponent.hpp"
#include <glm/glm.hpp>
#include "../linking.hpp"

namespace Component {
    /// %Component describing a spot light.
    class SpotLight : public SuperComponent {
        public:
        /// Create new spot light.
        ENGINE_API SpotLight();

        /// Save the component.
        /**
         * @return JSON value to be stored on disk.
         */
        ENGINE_API Json::Value Save() const override;

        /// Color.
        glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);

        /// Ambient coefficient.
        float ambientCoefficient = 0.f;

        /// Attenuation.
        float attenuation = 1.f;

        /// Intensity.
        float intensity = 1.f;

        /// Angle of the cone (in degrees).
        float coneAngle = 15.f;

        /// Whether the light should create shadows.
        bool shadow = false;

        /// Distance.
        float distance = 1.f;
    };
} // namespace Component
