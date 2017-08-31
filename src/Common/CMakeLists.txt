set(SRCS
        Log.cpp
    )

set(HEADERS
        Log.hpp
    )

create_directory_groups(${SRCS} ${HEADERS})

add_library(Common STATIC ${SRCS} ${HEADERS})
target_link_libraries(Common angelscript glew glfw ${GLFW_LIBRARIES} glm)
set_property(TARGET Common PROPERTY CXX_STANDARD 11)
set_property(TARGET Common PROPERTY CXX_STANDARD_REQUIRED ON)
