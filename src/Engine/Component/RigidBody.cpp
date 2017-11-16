#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "../Physics/GlmConversion.hpp"
#include "RigidBody.hpp"

#ifdef USINGMEMTRACK
#include <MemTrackInclude.hpp>
#endif

namespace Component {
    RigidBody::~RigidBody() {
        Destroy();
    }

    Json::Value RigidBody::Save() const {
        Json::Value component;
        component["mass"] = mass;
        component["friction"] = friction;
        component["rollingFriction"] = rollingFriction;
        component["spinningFriction"] = spinningFriction;
        component["cor"] = restitution;
        component["linearDamping"] = linearDamping;
        component["angularDamping"] = angularDamping;
        component["kinematic"] = kinematic;
        return component;
    }

    bool RigidBody::IsKinematic() const {
        return kinematic;
    }

    float RigidBody::GetFriction() const {
        return friction;
    }

    float RigidBody::GetRollingFriction() const {
        return rollingFriction;
    }

    float RigidBody::GetSpinningFriction() const {
        return spinningFriction;
    }

    float RigidBody::GetRestitution() const {
        return restitution;
    }

    float RigidBody::GetLinearDamping() const {
        return linearDamping;
    }

    float RigidBody::GetAngularDamping() const {
        return angularDamping;
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

        ghostObject = new btGhostObject();
    }

    void RigidBody::Destroy() {
        if (rigidBody) {
            delete rigidBody->getMotionState();
            delete rigidBody;
        }

        delete ghostObject;
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

    void RigidBody::SetRollingFriction(float friction) {
        rigidBody->setRollingFriction(friction);
        this->rollingFriction = friction;
    }

    void RigidBody::SetSpinningFriction(float friction) {
        rigidBody->setSpinningFriction(friction);
        this->spinningFriction = friction;
    }

    void RigidBody::SetRestitution(float cor) {
        rigidBody->setRestitution(cor);
        this->restitution = cor;
    }

    void RigidBody::SetLinearDamping(float damping) {
        rigidBody->setDamping(damping, angularDamping);
        this->linearDamping = damping;
    }

    void RigidBody::SetAngularDamping(float damping) {
        rigidBody->setDamping(linearDamping, damping);
        this->angularDamping = damping;
    }

    void RigidBody::MakeKinematic() {
        rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        kinematic = true;
        ghost = false;
    }

    void RigidBody::MakeDynamic() {
        rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
        kinematic = false;
        ghost = false;
    }

    void RigidBody::SetGhost(bool ghost) {
        this->ghost = ghost;
    }

    bool RigidBody::GetForceTransformSync() const {
        return forceTransformSync;
    }

    void RigidBody::SetForceTransformSync(bool sync) {
        forceTransformSync = sync;
    }

    bool RigidBody::GetHaltMovement() const {
        return haltMovement;
    }

    void RigidBody::SetHaltMovement(bool halt) {
        haltMovement = halt;
    }
}
