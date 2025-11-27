#pragma once
#include <cstdint>
#include <glm/vec3.hpp>


enum class ColliderType {
    Box,
    Sphere,
    Capsule
};

struct ColliderDesc {
    ColliderType type = ColliderType::Box;
    glm::vec3 size{ 1.0f, 1.0f, 1.0f };   // half-extents for box, radius for sphere, etc.
    glm::vec3 offset{ 0.0f };             // local offset from parent rigid body
};

class Collider {
public:
    Collider(const ColliderDesc& desc)
        : type(desc.type), size(desc.size), offset(desc.offset) {
    }

    ColliderType type;
    glm::vec3 size;
    glm::vec3 offset;
};
