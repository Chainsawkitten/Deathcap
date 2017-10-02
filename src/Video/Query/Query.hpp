#pragma once

#include <GL/glew.h>

namespace Video {

    /// Render surface containing framebuffers.
    class Query {
        public:

            /// Create new render surface.
            /**
             * @param target Specifies the target type of query object.
             */
            Query(GLenum target);
            
            /// Destructor.
            ~Query();
            
        private:



    };
}
