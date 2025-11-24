#include "PhysicsWorld.hpp"

PhysicsWorld::PhysicsWorld()
{
}

PhysicsWorld::~PhysicsWorld()
{
}

void PhysicsWorld::stepSimulation(float fixedDeltaTime)
{
}

RigidBodyID PhysicsWorld::createRigidBody(const RigidBodyDesc& desc)
{
	return RigidBodyID();
}

void PhysicsWorld::destroyRigidBody(RigidBodyID id)
{
}

RigidBody* PhysicsWorld::getRigidBody(RigidBodyID id)
{
	return nullptr;
}

ColliderID PhysicsWorld::createCollider(const ColliderDesc& desc)
{
	return ColliderID();
}

void PhysicsWorld::attachCollider(RigidBodyID body, ColliderID collider)
{
}

void PhysicsWorld::detachCollider(RigidBodyID body, ColliderID collider)
{
}

//bool PhysicsWorld::raycast(const Ray& ray, RaycastHit& hit)
//{
//	return false;
//}
//
//void PhysicsWorld::overlapQuery(const Shape& shape, const Transform& pose, std::vector<BodyID>& results)
//{
//}

void PhysicsWorld::setCollisionCallback(CollisionCallback cb)
{
}

void PhysicsWorld::setTriggerCallback(TriggerCallback cb)
{
}
