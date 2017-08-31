set(SRCS
        test.cpp
    )

set(HEADERS

    )

create_directory_groups(${SRCS} ${HEADERS})

add_library(Renderer STATIC ${SRCS} ${HEADERS})
target_link_libraries(Renderer Common glew glm)
set_property(TARGET Renderer PROPERTY CXX_STANDARD 11)
set_property(TARGET Renderer PROPERTY CXX_STANDARD_REQUIRED ON)
