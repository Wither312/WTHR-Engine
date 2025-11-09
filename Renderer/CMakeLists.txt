project(Renderer LANGUAGES CXX)

# Gather all source and header files
file(GLOB_RECURSE RENDERER_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE RENDERER_HEADERS CONFIGURE_DEPENDS include/*.hpp)

# Create static library
add_library(Renderer STATIC ${RENDERER_SOURCES} ${RENDERER_HEADERS})

# Include directories
target_include_directories(Renderer
    PUBLIC
        include
         ${CMAKE_CURRENT_SOURCE_DIR}/../WTHR/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glad/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glfw/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/spdlog/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/assimp/include
         ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/entt/src
)

# Link libraries
target_link_libraries(Renderer
    PUBLIC
        glad
        glfw
        imgui
        ImGuizmo
        spdlog::spdlog
        assimp
        EnTT
)

# MSVC-specific: enable parallel compilation
if(MSVC)
    target_compile_options(Renderer PRIVATE /MP)
endif()

# Optional: definitions
target_compile_definitions(Renderer PUBLIC LEAN_AND_MEAN)

# Precompiled header
target_precompile_headers(Renderer PRIVATE ../WTHR/include/pch.hpp)
