# Collect all source and header files
file(GLOB_RECURSE RUNTIME_SRC
    src/*.cpp
    src/*.cxx
)
file(GLOB_RECURSE RUNTIME_HDR
    include/*.hpp
    include/*.h
)
# Create executable
add_executable(Runtime
    ${RUNTIME_SRC}
    ${RUNTIME_HDR}
    
)

# Include directories
target_include_directories(Runtime PUBLIC include)

# Set C++ standard (inherits from root)
target_compile_features(Runtime PUBLIC cxx_std_20)

# MSVC multi-processor compile
if(MSVC)
    target_compile_options(Runtime PRIVATE /MP)
endif()

# Precompiled header
target_precompile_headers(WTHR PRIVATE ../Editor/include/pch.hpp)

# Link libraries
target_link_libraries(Runtime
    PRIVATE
        WTHR

        glfw
        glm
        spdlog
        nlohmann_json::nlohmann_json
        assimp
        imgui
        glad
        ImGuizmo
)
add_custom_command(TARGET Runtime POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Runtime>
)