#include <glm/glm.hpp>
#include <fstream>
#include "../Util/Node.hpp"

/// Animation transition node.
struct AnimationTransition : public Node {
    bool isStatic = true;
    int32_t transitionBoolIndex = -1;
    float transitionTime = 0.5f;
    float transitionProcess = 0.0f;

    /// Save the animation transition node.
    /**
     * @param file File to save to.
     */
    void Save(std::ofstream* file) override {
        Node::Save(file);
        file->write(reinterpret_cast<char*>(&isStatic), sizeof(bool));
        file->write(reinterpret_cast<char*>(&transitionBoolIndex), sizeof(int32_t));
        file->write(reinterpret_cast<char*>(&transitionTime), sizeof(float));
    }

    /// Load the animation transition node.
    /**
     * @param file File to load from.
     */
    void Load(std::ifstream* file) override {
        Node::Load(file);
        file->read(reinterpret_cast<char*>(&isStatic), sizeof(bool));
        file->read(reinterpret_cast<char*>(&transitionBoolIndex), sizeof(int32_t));
        file->read(reinterpret_cast<char*>(&transitionTime), sizeof(float));
    }
};
