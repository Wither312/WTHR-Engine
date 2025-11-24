cmake_minimum_required(VERSION 3.20)
project(WTHR LANGUAGES C CXX)

# Set C++ standard for the whole project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# MSVC multi-processor compile
if(MSVC)
    add_compile_options(/MP)
  
endif()
if(MSVC)
  set(CMAKE_C_COMPILER_LAUNCHER "")
  set(CMAKE_CXX_COMPILER_LAUNCHER "")
endif()

# Vendor libraries
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/entt)
add_subdirectory(vendor/json)
add_subdirectory(vendor/assimp)
add_subdirectory(vendor/Catch2)
add_subdirectory(vendor/sol2)

# Glad
add_library(glad STATIC vendor/glad/src/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/src/include)


# ImGui (manually add source files)
set(IMGUI_SRC
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui ${IMGUI_SRC})
target_include_directories(imgui PUBLIC vendor/imgui vendor/imgui/backends)
target_link_libraries(imgui PUBLIC glfw)

# ImGuizmo
add_library(ImGuizmo vendor/ImGuizmo/ImGuizmo.cpp)
target_include_directories(ImGuizmo PUBLIC vendor/ImGuizmo vendor/imgui)
target_link_libraries(ImGuizmo PUBLIC imgui glm)

# Add WTHR app
add_subdirectory(Physics)
add_subdirectory(WTHR)
add_subdirectory(Editor)
add_subdirectory(Runtime)
