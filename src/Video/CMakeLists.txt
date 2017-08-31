set(SRCS
        Renderer.cpp
        Lighting/Lighting.cpp
        Shader/Shader.cpp
        Shader/ShaderProgram.cpp
    )

set(HEADERS
        Renderer.hpp
        Lighting/Lighting.hpp
        Shader/Shader.hpp
        Shader/ShaderProgram.hpp
    )

create_directory_groups(${SRCS} ${HEADERS})

add_library(Video STATIC ${SRCS} ${HEADERS})
target_link_libraries(Video Common glew glm)
set_property(TARGET Video PROPERTY CXX_STANDARD 11)
set_property(TARGET Video PROPERTY CXX_STANDARD_REQUIRED ON)
