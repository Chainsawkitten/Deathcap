set(SRCS
        main.cpp
    )

set(HEADERS

    )

create_directory_groups(${SRCS} ${HEADERS})

add_executable(Game ${SRCS} ${HEADERS})
target_link_libraries(Game Engine)
if(TestMemory)
    target_compile_definitions(Game PRIVATE TESTMEMORY=1)
endif()

if(TestFrames)
    target_compile_definitions(Game PRIVATE TESTFRAMES=1)
endif()

set_property(TARGET Game PROPERTY CXX_STANDARD 11)
set_property(TARGET Game PROPERTY CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set_target_properties(Game PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Game")
endif()