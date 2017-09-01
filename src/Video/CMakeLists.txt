set(SRCS
        Renderer.cpp
        Culling/AxisAlignedBoundingBox.cpp
        Culling/Frustum.cpp
        Geometry/Geometry2D.cpp
        Geometry/Geometry3D.cpp
        Geometry/Rectangle.cpp
        Geometry/VertexType/SkinVertex.cpp
        Geometry/VertexType/StaticVertex.cpp
        Lighting/Lighting.cpp
        Shader/Shader.cpp
        Shader/ShaderProgram.cpp
        Texture/Texture.cpp
    )

set(HEADERS
        Renderer.hpp
        Culling/AxisAlignedBoundingBox.hpp
        Culling/Frustum.hpp
        Geometry/Geometry2D.hpp
        Geometry/Geometry3D.hpp
        Geometry/Rectangle.hpp
        Geometry/VertexType/SkinVertex.hpp
        Geometry/VertexType/StaticVertex.hpp
        Lighting/Light.hpp
        Lighting/Lighting.hpp
        Shader/Shader.hpp
        Shader/ShaderProgram.hpp
        Texture/Texture.hpp
    )

# Setup embedded source
set(EMBEDDED_INPUT
    ../../shaders/Deferred.frag
    ../../shaders/Post.vert
)

foreach(item ${EMBEDDED_INPUT})
    ProcessWithHeaderize(EMBEDDED_HEADER EMBEDDED_SOURCE ${item} ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

set_property(SOURCE ${SRCS} APPEND PROPERTY OBJECT_DEPENDS ${EMBEDDED_HEADER})

create_directory_groups(${SRCS} ${HEADERS})

add_library(Video STATIC ${SRCS} ${HEADERS} ${EMBEDDED_SOURCE})
target_link_libraries(Video Utility glew glm assimp jsoncpp_lib_static)
set_property(TARGET Video PROPERTY CXX_STANDARD 11)
set_property(TARGET Video PROPERTY CXX_STANDARD_REQUIRED ON)
