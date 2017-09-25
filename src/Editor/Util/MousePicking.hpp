#pragma once

#include "Engine\Entity\Entity.hpp"
/// Use mouse cursor to select objects in editor.

class MousePicking {

public:

    /// Fake constructor.
    MousePicking();

    /// Constructor.
    void CreateMousePicker(Entity * cam, glm::mat4 projection);

    /// Destructor.
    ~MousePicking();

    /// Return the vector for current ray.
    /**
    * @return A vec3 with values from current ray in world space. Normalized.
    */
    glm::vec3 GetCurrentRay();


    bool RayIntersectionOBB();

    /// Update the view matrix and recalculates the ray.
    void Update();

private:

    /// Where current ray direction points in world space.
    glm::vec3 currentRay;

    /// Projection matrix.
    glm::mat4 pMatrix;

    /// View matrix.
    glm::mat4 vMatrix;

    /// Entity to hold camera.
    Entity* camera;

    /// Return a vector
    /**
    * @return A vec3 with values from current ray in world space.
    */
    glm::vec3 CalculateRay();


    /// Return a vector with mouse coordinates in Normalized Device Coordinates.
    /**
    * @param X and Y coordinates.
    * @return A vec2 with values based on current mouse coordinates converted to NDC.
    */
    glm::vec2 GetNDC(double mouseX, double mouseY);
    
    /// Convert clip space coordinates to eye coordinates.
    glm::vec4 ConvertEyeCoords(glm::vec4 clipSpaceCoordinates);

    /// Convert eye coordinates to world coordinates.
    glm::vec3 ConvertWorldCoords(glm::vec4 eyeCoords);
};
