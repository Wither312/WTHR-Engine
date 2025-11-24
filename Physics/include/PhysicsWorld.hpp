#pragma once
#include <RigidBody.hpp>
#include <ColliderID.hpp>
#include <Callbacks.hpp>

class PhysicsWorld {
public:
    ~PhysicsWorld();
    PhysicsWorld();

    // --- Simulation ---
    void stepSimulation(float fixedDeltaTime);

    // --- Rigid body management ---
    RigidBodyID createRigidBody(const RigidBodyDesc& desc);
    void destroyRigidBody(RigidBodyID id);
    RigidBody* getRigidBody(RigidBodyID id);

    // --- Collider (shape) management ---
    ColliderID createCollider(const ColliderDesc& desc);
    void attachCollider(RigidBodyID body, ColliderID collider);
    void detachCollider(RigidBodyID body, ColliderID collider);

    // --- Raycasts & queries ---
   // bool raycast(const Ray& ray, RaycastHit& hit);
   // void overlapQuery(const Shape& shape, const Transform& pose, std::vector<BodyID>& results);

    // --- Events ---
    void setCollisionCallback(CollisionCallback cb);
    void setTriggerCallback(TriggerCallback cb);

private:
    // --- Internal modules (subsystems) ---
   // Broadphase           m_broadphase;
   // Narrowphase          m_narrowphase;
   // ConstraintSolver     m_solver;

    std::vector<RigidBody*>   m_rigidBodies;
    std::vector<Collider*>    m_colliders;

    // Active contact manifolds
 //  std::vector<ContactManifold> m_contacts;

    // Temp buffers for simulation
  //  SimulationContext     m_context;
};
