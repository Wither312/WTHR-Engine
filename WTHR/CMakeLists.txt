# Collect all source and header files
file(GLOB_RECURSE WTHR_SRC
    src/*.cpp
    src/*.cxx
)
file(GLOB_RECURSE WTHR_HDR
    include/*.hpp
    include/*.h
)
# Create executable
add_executable(WTHR
    ${WTHR_SRC}
    ${WTHR_HDR}
    src/pch.cpp      # precompiled header source
    include/pch.hpp  # precompiled header
)

# Include directories
target_include_directories(WTHR PUBLIC include)

# Set C++ standard (inherits from root)
target_compile_features(WTHR PUBLIC cxx_std_20)

# MSVC multi-processor compile
if(MSVC)
    target_compile_options(WTHR PRIVATE /MP)
endif()

# Precompiled header
target_precompile_headers(WTHR PRIVATE include/pch.hpp)

# Link libraries
target_link_libraries(WTHR
    PRIVATE
        glfw
        glm
        spdlog
        nlohmann_json::nlohmann_json
        assimp
        imgui
        glad
        ImGuizmo
        Renderer
)
