#pragma once

#include <GL/glew.h>
#include <cstdint>

namespace Video {

    /// Render surface containing framebuffers.
    class Query {
        public:
            /// Query types.
            enum Type
            {
                TIME_ELAPSED ///< TIME_ELAPSED Record the time that it takes for the GPU to execute all of the scoped commands between Begin() and End().
            };

            /// Create new query.
            /**
             * @param type Specifies the type of query object.
             */
            Query(Type type);
            
            /// Destructor.
            ~Query();

            /// Start query.
            void Begin();

            /// Stop query.
            void End();

            /// Resolve query result.
            /**
             * @return The result of query.
             */
            std::uint64_t Resolve() const;
            
        private:

            bool active;

            GLuint queries[2];
    };
}
