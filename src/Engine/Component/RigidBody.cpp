#include <btBulletDynamicsCommon.h>
#include "../Physics/GlmConversion.hpp"
#include "RigidBody.hpp"

namespace Component {
    RigidBody::~RigidBody() {
        Destroy();
    }

    Json::Value RigidBody::Save() const {
        Json::Value component;
        component["mass"] = mass;
        component["friction"] = friction;
        component["kinematic"] = kinematic;
        return component;
    }

    bool RigidBody::IsKinematic() const {
        return kinematic;
    }

    float RigidBody::GetFriction() const {
        return friction;
    }

    btRigidBody* RigidBody::GetBulletRigidBody() {
        return rigidBody;
    }

    void RigidBody::NewBulletRigidBody(float mass) {
        Destroy();

        this->mass = mass;

        // Motion states inform us of movement caused by physics so that we can
        // deal with those changes as needed.
        btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

        // Bullet treats zero mass as infinite, resulting in immovable objects.
        btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, nullptr, btVector3(0, 0, 0));
        rigidBody = new btRigidBody(constructionInfo);
        rigidBody->setUserPointer(this);
    }

    void RigidBody::Destroy() {
        if (rigidBody) {
            delete rigidBody->getMotionState();
            delete rigidBody;
        }
    }

    glm::vec3 RigidBody::GetPosition() const {
        btTransform trans;
        if (IsKinematic())
            rigidBody->getMotionState()->getWorldTransform(trans);
        else
            trans = rigidBody->getWorldTransform();

        return Physics::btToGlm(trans.getOrigin());
    }

    void RigidBody::SetPosition(const glm::vec3& pos) {
        if (IsKinematic()) {
            btTransform trans;
            rigidBody->getMotionState()->getWorldTransform(trans);
            trans.setOrigin(Physics::glmToBt(pos));
            rigidBody->getMotionState()->setWorldTransform(trans);
        } else {
            btTransform trans = rigidBody->getWorldTransform();
            trans.setOrigin(Physics::glmToBt(pos));
            rigidBody->setWorldTransform(trans);
        }
    }

    glm::quat RigidBody::GetOrientation() const {
        btTransform trans;
        if (IsKinematic())
            rigidBody->getMotionState()->getWorldTransform(trans);
        else
            trans = rigidBody->getWorldTransform();

        return Physics::btToGlm(trans.getRotation());
    }

    void RigidBody::SetOrientation(const glm::quat& rotation) {
        if (IsKinematic()) {
            btTransform trans;
            rigidBody->getMotionState()->getWorldTransform(trans);
            trans.setRotation(Physics::glmToBt(rotation));
            rigidBody->getMotionState()->setWorldTransform(trans);
        } else {
            btTransform trans = rigidBody->getWorldTransform();
            trans.setRotation(Physics::glmToBt(rotation));
            rigidBody->setWorldTransform(trans);
        }
    }

    float RigidBody::GetMass() {
        return mass;
    }

    void RigidBody::SetMass(float mass) {
        // Bullet provides a method on the shape that we can use to calculate
        // inertia.
        btVector3 inertia;
        rigidBody->getCollisionShape()->calculateLocalInertia(mass, inertia);
        rigidBody->setMassProps(mass, inertia);
        this->mass = mass;
    }

    void RigidBody::SetFriction(float friction) {
        rigidBody->setFriction(friction);
        this->friction = friction;
    }

    void RigidBody::MakeKinematic() {
        rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        kinematic = true;
    }

    void RigidBody::MakeDynamic() {
        rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
        kinematic = false;
    }

    bool RigidBody::GetForceTransformSync() const {
        return forceTransformSync;
    }

    void RigidBody::SetForceTransformSync(bool sync) {
        forceTransformSync = sync;
    }
}
