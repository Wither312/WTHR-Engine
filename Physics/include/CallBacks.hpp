#pragma once
#include <functional>
#include <cstdint>

using RigidBodyID = uint32_t;
using ColliderID = uint32_t;

// Collision callback signature
using CollisionCallback = std::function<void(RigidBodyID a, RigidBodyID b)>;

// Trigger callback signature
using TriggerCallback = std::function<void(RigidBodyID a, ColliderID trigger)>;

