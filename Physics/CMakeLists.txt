# Collect all source and header files
file(GLOB_RECURSE PHYS_SRC
    src/*.cpp
    src/*.cxx
)
file(GLOB_RECURSE PHYS_HDR
    include/*.hpp
    include/*.h
)
# Create executable
add_library(Physics
    STATIC
    ${PHYS_SRC}
    ${PHYS_HDR}
)
target_include_directories(Physics
    PUBLIC
        include
         ${CMAKE_CURRENT_SOURCE_DIR}/../Physics/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../Editor/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glad/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glfw/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/spdlog/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/assimp/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/entt/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/json/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/lua
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/sol2/include
)
# Include directories
target_include_directories(Physics PUBLIC include)

# Set C++ standard (inherits from root)
target_compile_features(Physics PUBLIC cxx_std_20)

# MSVC multi-processor compile
if(MSVC)
    target_compile_options(Physics PRIVATE /MP)
endif()

# Precompiled header

# Link libraries
target_link_libraries(Physics
    PRIVATE
    WTHR
      
)
