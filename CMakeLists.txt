cmake_minimum_required(VERSION 3.1.0)

project(HymnToBeauty)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(UNIX OR MINGW)
    option(HTB_DEBUG_INFO "Build Hymn to Beauty with DWARF debug data." OFF)
    if(HTB_DEBUG_INFO)
        set(DEBUG_DATA "-gdwarf-2")
    else()
        set(DEBUG_DATA "-s")
    endif()
    
    set(CMAKE_CXX_FLAGS "-g ${DEBUG_DATA} -O3 -static-libstdc++ -Wall -fno-strict-aliasing")
elseif(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Externals
add_subdirectory(externals)

# This function should be passed a list of all files in a target. It will automatically generate
# file groups following the directory hierarchy, so that the layout of the files in IDEs matches the
# one in the filesystem.
function(create_directory_groups)
    # Place any files that aren't in the source list in a separate group so that they don't get in
    # the way.
    source_group("Other Files" REGULAR_EXPRESSION ".")

    foreach(file_name ${ARGV})
        get_filename_component(dir_name "${file_name}" PATH)
        # Group names use '\' as a separator even though the entire rest of CMake uses '/'...
        string(REPLACE "/" "\\" group_name "${dir_name}")
        source_group("${group_name}" FILES "${file_name}")
    endforeach()
endfunction()

# Setup embedded source
set(EMBEDDED_INPUT
    shaders/Default2D.vert
    shaders/Default3D.vert
    shaders/Default3D.frag
    shaders/Deferred.frag
    shaders/DebugDrawing.vert
    shaders/DebugDrawing.frag
    shaders/EditorEntity.vert
    shaders/EditorEntity.geom
    shaders/EditorEntity.frag
    shaders/Font.vert
    shaders/Font.frag
    shaders/SingleColor2D.frag
    shaders/Skinning.vert
    shaders/Texture2D.frag
    shaders/Particle.vert
    shaders/Particle.geom
    shaders/Particle.frag
    shaders/Post.vert
    shaders/PostColor.frag
    shaders/PostCopy.frag
    shaders/PostDither.frag
    shaders/PostFog.frag
    shaders/PostFXAA.frag
    shaders/PostGamma.frag
    shaders/PostGlow.frag
    shaders/PostGlowBlur.frag
    assets/ABeeZee.ttf
    assets/Camera.png
    assets/DefaultDiffuse.png
    assets/DefaultNormal.png
    assets/DefaultSpecular.png
    assets/DefaultGlow.png
    assets/Light.png
    assets/ParticleAtlas.png
    assets/ParticleEmitter.png
    assets/SoundSource.png
)

# Headerize
add_subdirectory(externals/Headerize)
macro(ProcessWithHeaderize headervar sourcevar inputfile outdir)
    get_filename_component(basefile ${inputfile} NAME)
    set(outputfile ${outdir}/${basefile})
    set_source_files_properties(${outputfile} PROPERTIES HEADER_ONLY TRUE)
    add_custom_command(
        OUTPUT ${outputfile}.hpp ${outputfile}.cpp
        COMMAND Headerize ${inputfile} ${outputfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${inputfile}
        COMMENT "Generating headers for ${inputfile}"
    )
    list(APPEND ${headervar} ${outputfile}.hpp)
    list(APPEND ${sourcevar} ${outputfile}.cpp)
endmacro()

foreach(item ${EMBEDDED_INPUT})
    ProcessWithHeaderize(EMBEDDED_HEADER EMBEDDED_SOURCE ${item} ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

# Doxygen documentation
option(HymnToBeautyBuildDocs "Build the doxygen documentation" OFF)
find_package(Doxygen)
if(DOXYGEN_FOUND AND HymnToBeautyBuildDocs)
    add_custom_target(EngineDoc ALL
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/EngineDoxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
    add_custom_target(EditorDoc ALL
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/EditorDoxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
    add_custom_target(GameDoc ALL
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/GameDoxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif()

include_directories(src)


## TARGETS ##

## Engine ##
set(ENGINE_SRCS
        externals/AngelScript/sdk/add_on/scriptbuilder/scriptbuilder.cpp
        externals/AngelScript/sdk/add_on/scriptmath/scriptmath.cpp
        externals/AngelScript/sdk/add_on/scriptstdstring/scriptstdstring.cpp
        externals/AngelScript/sdk/add_on/scriptstdstring/scriptstdstring_utils.cpp
        externals/imgui/imgui.cpp
        externals/imgui/imgui_demo.cpp
        externals/imgui/imgui_draw.cpp
        src/Engine/Hymn.cpp
        src/Engine/MainWindow.cpp
        src/Engine/RenderTarget.cpp
        src/Engine/Audio/SoundBuffer.cpp
        src/Engine/Audio/VorbisFile.cpp
        src/Engine/Component/Animation.cpp
        src/Engine/Component/DirectionalLight.cpp
        src/Engine/Component/Lens.cpp
        src/Engine/Component/Listener.cpp
        src/Engine/Component/Material.cpp
        src/Engine/Component/Mesh.cpp
        src/Engine/Component/ParticleEmitter.cpp
        src/Engine/Component/Physics.cpp
        src/Engine/Component/PointLight.cpp
        src/Engine/Component/Script.cpp
        src/Engine/Component/SoundSource.cpp
        src/Engine/Component/SpotLight.cpp
        src/Engine/Component/SuperComponent.cpp
        src/Engine/Entity/Entity.cpp
        src/Engine/Entity/World.cpp
        src/Engine/Font/Font.cpp
        src/Engine/Lighting/DeferredLighting.cpp
        src/Engine/Manager/Managers.cpp
        src/Engine/Manager/DebugDrawingManager.cpp
        src/Engine/Manager/ParticleManager.cpp
        src/Engine/Manager/ProfilingManager.cpp
        src/Engine/Manager/PhysicsManager.cpp
        src/Engine/Manager/RenderManager.cpp
        src/Engine/Manager/ResourceManager.cpp
        src/Engine/Manager/ScriptManager.cpp
        src/Engine/Manager/SoundManager.cpp
        src/Engine/Physics/AxisAlignedBoundingBox.cpp
        src/Engine/Physics/Frustum.cpp
        src/Engine/Physics/Rectangle.cpp
        src/Engine/PostProcessing/ColorFilter.cpp
        src/Engine/PostProcessing/FogFilter.cpp
        src/Engine/PostProcessing/FXAAFilter.cpp
        src/Engine/PostProcessing/GammaCorrectionFilter.cpp
        src/Engine/PostProcessing/GlowFilter.cpp
        src/Engine/PostProcessing/GlowBlurFilter.cpp
        src/Engine/PostProcessing/PostProcessing.cpp
        src/Engine/RenderProgram/SkinRenderProgram.cpp
        src/Engine/RenderProgram/StaticRenderProgram.cpp
        src/Engine/Util/FileSystem.cpp
        src/Engine/Util/Input.cpp
        src/Engine/Util/Json.cpp
        src/Engine/Util/Profiling.cpp
        src/Engine/Util/Settings.cpp
        src/Engine/Util/Log.cpp
        src/Engine/Shader/Shader.cpp
        src/Engine/Shader/ShaderProgram.cpp
        src/Engine/Geometry/Animation.cpp
        src/Engine/Geometry/Cube.cpp
        src/Engine/Geometry/Geometry2D.cpp
        src/Engine/Geometry/Geometry3D.cpp
        src/Engine/Geometry/MathFunctions.cpp
        src/Engine/Geometry/Model.cpp
        src/Engine/Geometry/Rectangle.cpp
        src/Engine/Geometry/RiggedModel.cpp
        src/Engine/Geometry/Skeleton.cpp
        src/Engine/Geometry/StaticModel.cpp
        src/Engine/Geometry/VertexType/SkinVertex.cpp
        src/Engine/Geometry/VertexType/StaticVertex.cpp
        src/Engine/Texture/Texture.cpp
        src/Engine/Texture/Texture2D.cpp
        src/Engine/Script/ScriptFile.cpp
        src/Engine/Input/Input.cpp
    )

set(ENGINE_HEADERS
        src/Engine/Hymn.hpp
        src/Engine/MainWindow.hpp
        src/Engine/RenderTarget.hpp
        src/Engine/Audio/SoundBuffer.hpp
        src/Engine/Audio/SoundFile.hpp
        src/Engine/Audio/VorbisFile.hpp
        src/Engine/Component/Animation.hpp
        src/Engine/Component/DirectionalLight.hpp
        src/Engine/Component/Lens.hpp
        src/Engine/Component/Listener.hpp
        src/Engine/Component/Material.hpp
        src/Engine/Component/Mesh.hpp
        src/Engine/Component/ParticleEmitter.hpp
        src/Engine/Component/Physics.hpp
        src/Engine/Component/PointLight.hpp
        src/Engine/Component/Script.hpp
        src/Engine/Component/SoundSource.hpp
        src/Engine/Component/SpotLight.hpp
        src/Engine/Component/SuperComponent.hpp
        src/Engine/Entity/Entity.hpp
        src/Engine/Entity/World.hpp
        src/Engine/Font/Font.hpp
        src/Engine/Lighting/DeferredLighting.hpp
        src/Engine/Manager/Managers.hpp
        src/Engine/Manager/DebugDrawingManager.hpp
        src/Engine/Manager/ParticleManager.hpp
        src/Engine/Manager/ProfilingManager.hpp
        src/Engine/Manager/PhysicsManager.hpp
        src/Engine/Manager/RenderManager.hpp
        src/Engine/Manager/ResourceManager.hpp
        src/Engine/Manager/ScriptManager.hpp
        src/Engine/Manager/SoundManager.hpp
        src/Engine/Physics/AxisAlignedBoundingBox.hpp
        src/Engine/Physics/Frustum.hpp
        src/Engine/Physics/Rectangle.hpp
        src/Engine/PostProcessing/ColorFilter.hpp
        src/Engine/PostProcessing/Filter.hpp
        src/Engine/PostProcessing/FogFilter.hpp
        src/Engine/PostProcessing/FXAAFilter.hpp
        src/Engine/PostProcessing/GammaCorrectionFilter.hpp
        src/Engine/PostProcessing/GlowFilter.hpp
        src/Engine/PostProcessing/GlowBlurFilter.hpp
        src/Engine/PostProcessing/PostProcessing.hpp
        src/Engine/RenderProgram/SkinRenderProgram.hpp
        src/Engine/RenderProgram/StaticRenderProgram.hpp
        src/Engine/Util/FileSystem.hpp
        src/Engine/Util/Input.hpp
        src/Engine/Util/Json.hpp
        src/Engine/Util/Profiling.hpp
        src/Engine/Util/Settings.hpp
        src/Engine/Util/Log.hpp
        src/Engine/Shader/Shader.hpp
        src/Engine/Shader/ShaderProgram.hpp
        src/Engine/Geometry/Animation.hpp
        src/Engine/Geometry/Cube.hpp
        src/Engine/Geometry/Geometry2D.hpp
        src/Engine/Geometry/Geometry3D.hpp
        src/Engine/Geometry/MathFunctions.hpp
        src/Engine/Geometry/Model.hpp
        src/Engine/Geometry/Rectangle.hpp
        src/Engine/Geometry/RiggedModel.hpp
        src/Engine/Geometry/Skeleton.hpp
        src/Engine/Geometry/StaticModel.hpp
        src/Engine/Geometry/VertexType/SkinVertex.hpp
        src/Engine/Geometry/VertexType/StaticVertex.hpp
        src/Engine/Texture/Texture.hpp
        src/Engine/Texture/Texture2D.hpp
        src/Engine/Script/ScriptFile.hpp
        src/Engine/Input/Input.hpp

    )

create_directory_groups(${ENGINE_SRCS} ${ENGINE_HEADERS})

set_property(SOURCE ${ENGINE_SRCS} APPEND PROPERTY OBJECT_DEPENDS ${EMBEDDED_HEADER})

add_library(Engine STATIC ${ENGINE_SRCS} ${ENGINE_HEADERS} ${EMBEDDED_SOURCE})
target_link_libraries(Engine glew glfw ${GLFW_LIBRARIES} glm imgui stb simpleini jsoncpp_lib_static ${OPENAL_LIB} angelscript assimp)
set_property(TARGET Engine PROPERTY CXX_STANDARD 11)
set_property(TARGET Engine PROPERTY CXX_STANDARD_REQUIRED ON)



## Editor ##
set(EDITOR_SRCS
        src/Editor/main.cpp
        src/Editor/Editor.cpp
        src/Editor/ImGui/OpenGLImplementation.cpp
        src/Editor/ImGui/Splitter.cpp
        src/Editor/ImGui/Theme.cpp
        src/Editor/ImGui/GuiHelpers.cpp
        src/Editor/GUI/FiltersWindow.cpp
        src/Editor/GUI/SelectHymnWindow.cpp
        src/Editor/GUI/InputWindow.cpp
        src/Editor/GUI/FileSelector.cpp
        src/Editor/GUI/ResourceList.cpp
        src/Editor/GUI/SettingsWindow.cpp
        src/Editor/GUI/Editors/EntityEditor.cpp
        src/Editor/GUI/Editors/ModelEditor.cpp
        src/Editor/GUI/Editors/SceneEditor.cpp
        src/Editor/GUI/Editors/SoundEditor.cpp
        src/Editor/GUI/Editors/ScriptEditor.cpp
        src/Editor/GUI/Editors/TextureEditor.cpp
        src/Editor/Util/EditorSettings.cpp
    )

set(EDITOR_HEADERS
        src/Editor/Editor.hpp
        src/Editor/ImGui/OpenGLImplementation.hpp
        src/Editor/ImGui/Splitter.hpp
        src/Editor/ImGui/Theme.hpp
        src/Editor/ImGui/GuiHelpers.hpp
        src/Editor/GUI/FiltersWindow.hpp
        src/Editor/GUI/SelectHymnWindow.hpp
        src/Editor/GUI/InputWindow.hpp
        src/Editor/GUI/FileSelector.hpp
        src/Editor/GUI/ResourceList.hpp
        src/Editor/GUI/SettingsWindow.hpp
        src/Editor/GUI/Editors/EntityEditor.hpp
        src/Editor/GUI/Editors/ModelEditor.hpp
        src/Editor/GUI/Editors/SceneEditor.hpp
        src/Editor/GUI/Editors/SoundEditor.hpp
        src/Editor/GUI/Editors/ScriptEditor.hpp
        src/Editor/GUI/Editors/TextureEditor.hpp
        src/Editor/Util/EditorSettings.hpp
    )

create_directory_groups(${EDITOR_SRCS} ${EDITOR_HEADERS})

add_executable(Editor ${EDITOR_SRCS} ${EDITOR_HEADERS})
target_link_libraries(Editor Engine)
set_property(TARGET Editor PROPERTY CXX_STANDARD 11)
set_property(TARGET Editor PROPERTY CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
