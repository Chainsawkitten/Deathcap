set(SRCS
        Log.cpp
    )

set(HEADERS
        linking.hpp
        LockBox.hpp
        Log.hpp
    )

create_directory_groups(${SRCS} ${HEADERS})

# Logtesting define
option(USELOGTESTING "Log throws exceptions when logging to error channel." OFF)

if(USELOGTESTING)
    add_definitions(-DLOGTESTING)
endif()

add_library(Utility SHARED ${SRCS} ${HEADERS})
target_link_libraries(Utility glm)
set_property(TARGET Utility PROPERTY CXX_STANDARD 11)
set_property(TARGET Utility PROPERTY CXX_STANDARD_REQUIRED ON)
