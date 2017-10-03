#pragma once

#include <fstream>
#include <cstdint>
#include <assert.h>
#include <vector>
#include <Video/Geometry/VertexType/StaticVertex.hpp>
#include <Video/Geometry/VertexType/SkinVertex.hpp>

namespace Geometry {
    /// Handler for .asset format for animation.
    /**
     * Start by using the Open() function.
     * The Open() function requries a filepath and
     * a mode READ/WRITE.
     * End by using the Close() function.
     */
    class AssetFileAnimation {
        public:
            /// Mode to use class. READ/WRITE.
            enum Mode {
                READ = 0,
                WRITE = 1
            };

            /// Constructor.
            AssetFileAnimation();

            /// Destructor.
            ~AssetFileAnimation();

            /// Open a .asset file.
            /**
            * @param filepath Path of the file.
            * @param mode Use READ or WRITE.
            */
            bool Open(const char* filepath, Mode mode = READ);

            /// Close the opened file and clear data.
            void Close();

            /// Clear current data.
            void Clear();

        private:
            std::ifstream rFile;
            std::ofstream wFile;
    };
}