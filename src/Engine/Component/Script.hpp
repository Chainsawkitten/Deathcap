#pragma once

#include <vector>
#include <map>
#include <string.h>
#include "SuperComponent.hpp"
#include "../linking.hpp"

class ScriptFile;
class asIScriptObject;

namespace Component {
    /// %Component controlled by a script.
    class Script : public SuperComponent {
        public:
            /// Create new script.
            ENGINE_API Script();
            
            /// Destructor.
            ENGINE_API ~Script() final;
            
            /// Save the component.
            /**
             * @return JSON value to be stored on disk.
             */
            ENGINE_API Json::Value Save() const override;
            
            /// Whether the script component has been initialized.
            bool initialized = false;
            
            /// The script file.
            ScriptFile* scriptFile = nullptr;
            
            /// The instance of the script class.
            asIScriptObject* instance = nullptr;

            /// Add a property to the propertyMap.
            /**
             * @param name The name of the property.
             * @param type The asTypeID of the property.
             * @param size The size in number of bytes of the property.
             * @param data A pointer to the data to store.
             */
            ENGINE_API void AddToPropertyMap(std::string name, int type, int size, void* data);

            /// Add a property to the propertyMap.
            /**
             * @param name The name of the property.
             * @param type The asTypeID of the property.
             * @param target A pointer that points where to copy the data.
             */
            ENGINE_API void CopyDataFromPropertyMap(std::string name, void* target);

            /// Add a property to the propertyMap.
            /**
             * @param name The name of the property.
             * @return A pointer to the data of the property.
             */
            ENGINE_API void* GetDataFromPropertyMap(std::string name);

            /// Save the component.
            /**
             * @param name The name of the property.
             * @param type The asTypeID of the property.
             * @return Is there a property with the provided name and type in propertyMap.
             */  
            ENGINE_API bool isInPropertyMap(std::string name, int type);

            /// Clears the property map.
            ENGINE_API void ClearPropertyMap();

        private:

            class Property {
                
            public:

                Property() {

                    typeID = -1;
                    size = -1;
                    data = nullptr;

                }

                Property(int _typeID, int _size, void* _data) {

                    typeID = _typeID;
                    size = _size;
                    data = malloc(size);
                    std::memcpy(data, _data, size);

                }

                ~Property() {
                    std::free(data);
                }

                int typeID;
                int size;
                void* data;

            };

            //Map containing the properties, maps a struct of a value, it's type, and size to a name.
            std::map<std::string, Property> propertyMap;

    };
}
