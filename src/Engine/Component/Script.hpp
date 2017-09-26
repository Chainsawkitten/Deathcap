#pragma once

#include "SuperComponent.hpp"

class ScriptFile;
class asIScriptObject;

namespace Component {
    /// %Component controlled by a script.
    class Script : public SuperComponent {
        public:
            /// Create new script.
            Script();
            
            /// Destructor.
            ~Script() final;
            
            /// Save the component.
            /**
             * @return JSON value to be stored on disk.
             */
            Json::Value Save() const override;
            
            /// Load component from JSON node.
            /**
             * @param node JSON node to load from.
             */
            void Load(const Json::Value& node) override;
            
            /// Whether the script component has been initialized.
            bool initialized = false;
            
            /// The script file.
            ScriptFile* scriptFile = nullptr;
            
            /// The instance of the script class.
            asIScriptObject* instance = nullptr;

    };
}
