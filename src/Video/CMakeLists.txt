set(SRCS
        Renderer.cpp
        Culling/AxisAlignedBoundingBox.cpp
        Culling/Frustum.cpp
        Geometry/Geometry2D.cpp
        Geometry/Rectangle.cpp
        Lighting/Lighting.cpp
        Shader/Shader.cpp
        Shader/ShaderProgram.cpp
    )

set(HEADERS
        Renderer.hpp
        Culling/AxisAlignedBoundingBox.hpp
        Culling/Frustum.hpp
        Geometry/Geometry2D.hpp
        Geometry/Rectangle.hpp
        Lighting/Light.hpp
        Lighting/Lighting.hpp
        Shader/Shader.hpp
        Shader/ShaderProgram.hpp
    )

# Setup embedded source
set(EMBEDDED_INPUT
    ../../shaders/Default2D.vert
    ../../shaders/Default3D.vert
    ../../shaders/Default3D.frag
    ../../shaders/Deferred.frag
    ../../shaders/DebugDrawing.vert
    ../../shaders/DebugDrawing.frag
    ../../shaders/EditorEntity.vert
    ../../shaders/EditorEntity.geom
    ../../shaders/EditorEntity.frag
    ../../shaders/Font.vert
    ../../shaders/Font.frag
    ../../shaders/Skinning.vert
    ../../shaders/Texture2D.frag
    ../../shaders/Particle.vert
    ../../shaders/Particle.geom
    ../../shaders/Particle.frag
    ../../shaders/Post.vert
    ../../shaders/PostColor.frag
    ../../shaders/PostCopy.frag
    ../../shaders/PostDither.frag
    ../../shaders/PostFog.frag
    ../../shaders/PostFXAA.frag
    ../../shaders/PostGamma.frag
    ../../shaders/PostGlow.frag
    ../../shaders/PostGlowBlur.frag
    ../../assets/ABeeZee.ttf
    ../../assets/Camera.png
    ../../assets/DefaultDiffuse.png
    ../../assets/DefaultNormal.png
    ../../assets/DefaultSpecular.png
    ../../assets/DefaultGlow.png
    ../../assets/Light.png
    ../../assets/ParticleAtlas.png
    ../../assets/ParticleEmitter.png
    ../../assets/SoundSource.png
)

foreach(item ${EMBEDDED_INPUT})
    ProcessWithHeaderize(EMBEDDED_HEADER EMBEDDED_SOURCE ${item} ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

set_property(SOURCE ${SRCS} APPEND PROPERTY OBJECT_DEPENDS ${EMBEDDED_HEADER})

create_directory_groups(${SRCS} ${HEADERS})

add_library(Video STATIC ${SRCS} ${HEADERS} ${EMBEDDED_SOURCE})
target_link_libraries(Video Common glew glm)
set_property(TARGET Video PROPERTY CXX_STANDARD 11)
set_property(TARGET Video PROPERTY CXX_STANDARD_REQUIRED ON)
