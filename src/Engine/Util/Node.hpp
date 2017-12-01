#pragma once

#include <fstream>
#include <glm/glm.hpp>
#include "../linking.hpp"

/// Base node.
struct Node {
    /// Destructor.
    ENGINE_API virtual ~Node();

    /// Save node data.
    /**
     * @param file File to save to.
     */
    ENGINE_API virtual void Save(std::ofstream* file);

    /// Load node data.
    /**
     * @param file File to load from.
     */
    ENGINE_API virtual void Load(std::ifstream* file);

    /// Node index.
    uint32_t index;

    /// Name of the node, only used in editor.
    char name[128];
    
    /// The number of input slots.
    uint32_t numInputSlots = 0;
    
    /// Input slot index.
    uint32_t inputIndex[8];
    
    /// Number of output slots.
    uint32_t numOutputSlots = 0;
    
    /// Output slot index.
    uint32_t outputIndex[8];
    
    /// Position of the node, only used in editor. 
    glm::vec2 pos = glm::vec2(0.0f);
    
    /// Size of the node, only used in editor.
    glm::vec2 size = glm::vec2(250, 100);
};
