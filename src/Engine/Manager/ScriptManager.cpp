#include "ScriptManager.hpp"

#include <angelscript.h>
#include <scriptbuilder/scriptbuilder.h>
#include <scriptmath/scriptmath.h>
#include <scriptstdstring/scriptstdstring.h>
#include <Utility/Log.hpp>
#include <map>
#include <typeindex>
#include <sstream>
#include <cstring>

#include "../Util/FileSystem.hpp"
#include "../Util/Input.hpp"
#include "../Util/RayIntersection.hpp"
#include "../Util/MousePicking.hpp"
#include "../Hymn.hpp"
#include "../Entity/World.hpp"
#include "../Entity/Entity.hpp"
#include "../Component/Script.hpp"
#include "../Component/AnimationController.hpp"
#include "../Component/DirectionalLight.hpp"
#include "../Component/Lens.hpp"
#include "../Component/Listener.hpp"
#include "../Component/Mesh.hpp"
#include "../Component/PointLight.hpp"
#include "../Component/RigidBody.hpp"
#include "../Component/SoundSource.hpp"
#include "../Component/SpotLight.hpp"
#include "../Component/VRDevice.hpp"
#include "../Input/Input.hpp"
#include "../Script/ScriptFile.hpp"
#include "MainWindow.hpp"

#include "Managers.hpp"
#include "DebugDrawingManager.hpp"
#include "PhysicsManager.hpp"
#include "ResourceManager.hpp"
#include "RenderManager.hpp"

using namespace Component;

void AngelScriptMessageCallback(const asSMessageInfo* message, void* param) {
    Log() << message->section << " (" << message->row << ", " << message->col << " : ";
    
    switch (message->type) {
    case asMSGTYPE_ERROR:
        Log() << "Error";
        break;
    case asMSGTYPE_INFORMATION:
        Log() << "Information";
        break;
    case asMSGTYPE_WARNING:
        Log() << "Warning";
        break;
    }
    
    Log() << " : " << message->message << "\n";
}

// An example line callback
void AngelScriptDebugLineCallback(asIScriptContext *ctx, const std::map<std::string, std::set<int>>* breakpoints){
    const char *scriptSection;
    int line = ctx->GetLineNumber(0, 0, &scriptSection);

    std::string fileName(scriptSection);
    fileName = fileName.substr(fileName.find_last_of("/") + 1);

    // Determine if we have reached a break point
    if (breakpoints->find(fileName) != breakpoints->end() && breakpoints->at(fileName).find(line) != breakpoints->at(fileName).end()) {
        // A break point has been reached so the execution of the script should be suspended
        // Show the call stack
        for (asUINT n = 0; n < ctx->GetCallstackSize(); n++) {
            asIScriptFunction *func;
            const char *scriptSection;
            int line, column;
            func = ctx->GetFunction(n);
            line = ctx->GetLineNumber(n, &column, &scriptSection);
            printf("%s:%s:%d,%d\n", scriptSection,
                func->GetDeclaration(),
                line, column);
        }
    }
}

void print(const std::string& message) {
    Log() << message;
}

void RegisterUpdate() {
    Managers().scriptManager->RegisterUpdate(Managers().scriptManager->currentEntity);
}

void RegisterTriggerEnterHelper(Component::RigidBody* triggerBody, Component::RigidBody* object, const std::string& methodName) {
    Managers().scriptManager->RegisterTriggerEnter(Managers().scriptManager->currentEntity, triggerBody, object, methodName);
}

void RegisterTriggerRetainHelper(Component::RigidBody* triggerBody, Component::RigidBody* object, const std::string& methodName) {
    Managers().scriptManager->RegisterTriggerRetain(Managers().scriptManager->currentEntity, triggerBody, object, methodName);
}

void RegisterTriggerLeaveHelper(Component::RigidBody* triggerBody, Component::RigidBody* object, const std::string& methodName) {
    Managers().scriptManager->RegisterTriggerLeave(Managers().scriptManager->currentEntity, triggerBody, object, methodName);
}

bool ButtonInput(int buttonIndex) {
    if (Managers().vrManager->Active())
        return Input::GetInstance().CheckVRButton(buttonIndex, Managers().scriptManager->currentEntity->GetComponent<VRDevice>());
    else
        return Input::GetInstance().CheckButton(buttonIndex);
}

glm::vec2 GetCursorXY() {
    return Input()->GetCursorXY();
}

void SendMessage(Entity* recipient, int type) {
    Managers().scriptManager->SendMessage(recipient, type);
}

void RestartScene() {
    Hymn().restart = true;
}

bool IsIntersect(Entity* checker, Entity* camera) {
    MousePicking mousePicker = MousePicking(camera, camera->GetComponent<Component::Lens>()->GetProjection(glm::vec2(MainWindow::GetInstance()->GetSize().x, MainWindow::GetInstance()->GetSize().y)));
    mousePicker.Update();
    RayIntersection rayIntersector;
    float intersectDistance;
    if (rayIntersector.RayOBBIntersect(camera->GetWorldPosition(), mousePicker.GetCurrentRay(),
        checker->GetComponent<Component::Mesh>()->geometry->GetAxisAlignedBoundingBox(),
        checker->GetModelMatrix(), intersectDistance)) {
        if (intersectDistance < 10.0f)
            return true;
        return false;
    }
    return false;
}

bool IsVRActive() {
    return Managers().vrManager->Active();
}

void vec2Constructor(float x, float y, void* memory) {
    glm::vec2* vec = static_cast<glm::vec2*>(memory);
    vec->x = x;
    vec->y = y;
}

void vec3Constructor(float x, float y, float z, void* memory) {
    glm::vec3* vec = static_cast<glm::vec3*>(memory);
    vec->x = x;
    vec->y = y;
    vec->z = z;
}

void vec4Constructor(float x, float y, float z, float w, void* memory) {
    glm::vec4* vec = static_cast<glm::vec4*>(memory);
    vec->x = x;
    vec->y = y;
    vec->z = z;
    vec->w = w;
}

template<typename type> void glmConstructor(void* memory) {
    *static_cast<type*>(memory) = type();
}

template<typename type> type glmAdd(const type& a, const void* memory) {
    return *static_cast<const type*>(memory) + a;
}

template<typename type> type glmSub(const type& a, const void* memory) {
    return *static_cast<const type*>(memory) - a;
}

template<typename S, typename T> S glmMul(T a, const void* memory) {
    return *static_cast<const S*>(memory) * a;
}

template<typename type> type glmMulR(float a, const void* memory) {
    return a * *static_cast<const type*>(memory);
}

template<typename type> type glmDiv(float a, const void* memory) {
    return *static_cast<const type*>(memory) / a;
}

template<typename type> type glmDivR(float a, const void* memory) {
    return a / *static_cast<const type*>(memory);
}

template<typename type> type glmNeg(const void* memory) {
    return -*static_cast<const type*>(memory);
}

glm::vec3 mat3MulVec3(const glm::vec3& a, const void* memory) {
    return *static_cast<const glm::mat3*>(memory) * a;
}

glm::vec4 mat4MulVec4(const glm::vec4& a, const void* memory) {
    return *static_cast<const glm::mat4*>(memory) * a;
}

ScriptManager::ScriptManager() {
    // Create the script engine
    engine = asCreateScriptEngine();
    
    // Set the message callback to receive information on errors in human readable form.
    engine->SetMessageCallback(asFUNCTION(AngelScriptMessageCallback), 0, asCALL_CDECL);
  
    // Register add-ons.
    RegisterStdString(engine);
    RegisterScriptMath(engine);

    engine->RegisterEnum("input");
    
    // Register GLM types.
    engine->RegisterObjectType("vec2", sizeof(glm::vec2), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<glm::vec2>());
    engine->RegisterObjectProperty("vec2", "float x", asOFFSET(glm::vec2, x));
    engine->RegisterObjectProperty("vec2", "float y", asOFFSET(glm::vec2, y));
    engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTION(vec2Constructor), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec2", "vec2 opAdd(const vec2 &in) const", asFUNCTIONPR(glmAdd, (const glm::vec2&, const void*), glm::vec2), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec2", "vec2 opSub(const vec2 &in) const", asFUNCTIONPR(glmSub, (const glm::vec2&, const void*), glm::vec2), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec2", "vec2 opMul(float) const", asFUNCTIONPR(glmMul, (float, const void*), glm::vec2), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec2", "vec2 opMul_r(float) const", asFUNCTIONPR(glmMulR, (float, const void*), glm::vec2), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec2", "vec2 opDiv(float) const", asFUNCTIONPR(glmDiv, (float, const void*), glm::vec2), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec2", "vec2 opDiv_r(float) const", asFUNCTIONPR(glmDivR, (float, const void*), glm::vec2), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec2", "vec2& opAddAssign(const vec2 &in) const", asMETHODPR(glm::vec2, operator+=, (const glm::vec2&), glm::vec2&), asCALL_THISCALL);
    engine->RegisterObjectMethod("vec2", "vec2& opSubAssign(const vec2 &in) const", asMETHODPR(glm::vec2, operator-=, (const glm::vec2&), glm::vec2&), asCALL_THISCALL);
    engine->RegisterObjectMethod("vec2", "vec2& opMulAssign(float) const", asMETHODPR(glm::vec2, operator*=, (float), glm::vec2&), asCALL_THISCALL);
    engine->RegisterObjectMethod("vec2", "vec2& opDivAssign(float) const", asMETHODPR(glm::vec2, operator/=, (float), glm::vec2&), asCALL_THISCALL);
    engine->RegisterObjectMethod("vec2", "vec2 opNeg() const", asFUNCTIONPR(glmNeg, (const void*), glm::vec2), asCALL_CDECL_OBJLAST);
    
    engine->RegisterObjectType("vec3", sizeof(glm::vec3), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<glm::vec3>());
    engine->RegisterObjectProperty("vec3", "float x", asOFFSET(glm::vec3, x));
    engine->RegisterObjectProperty("vec3", "float y", asOFFSET(glm::vec3, y));
    engine->RegisterObjectProperty("vec3", "float z", asOFFSET(glm::vec3, z));
    engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTION(vec3Constructor), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec3", "vec3 opAdd(const vec3 &in) const", asFUNCTIONPR(glmAdd, (const glm::vec3&, const void*), glm::vec3), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec3", "vec3 opSub(const vec3 &in) const", asFUNCTIONPR(glmSub, (const glm::vec3&, const void*), glm::vec3), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec3", "vec3 opMul(float) const", asFUNCTIONPR(glmMul, (float, const void*), glm::vec3), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec3", "vec3 opMul_r(float) const", asFUNCTIONPR(glmMulR, (float, const void*), glm::vec3), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec3", "vec3 opDiv(float) const", asFUNCTIONPR(glmDiv, (float, const void*), glm::vec3), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec3", "vec3 opDiv_r(float) const", asFUNCTIONPR(glmDivR, (float, const void*), glm::vec3), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec3", "vec3& opAddAssign(const vec3 &in) const", asMETHODPR(glm::vec3, operator+=, (const glm::vec3&), glm::vec3&), asCALL_THISCALL);
    engine->RegisterObjectMethod("vec3", "vec3& opSubAssign(const vec3 &in) const", asMETHODPR(glm::vec3, operator-=, (const glm::vec3&), glm::vec3&), asCALL_THISCALL);
    engine->RegisterObjectMethod("vec3", "vec3& opMulAssign(float) const", asMETHODPR(glm::vec3, operator*=, (float), glm::vec3&), asCALL_THISCALL);
    engine->RegisterObjectMethod("vec3", "vec3& opDivAssign(float) const", asMETHODPR(glm::vec3, operator/=, (float), glm::vec3&), asCALL_THISCALL);
    engine->RegisterObjectMethod("vec3", "vec3 opNeg() const", asFUNCTIONPR(glmNeg, (const void*), glm::vec3), asCALL_CDECL_OBJLAST);
    
    engine->RegisterObjectType("vec4", sizeof(glm::vec4), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<glm::vec4>());
    engine->RegisterObjectProperty("vec4", "float x", asOFFSET(glm::vec4, x));
    engine->RegisterObjectProperty("vec4", "float y", asOFFSET(glm::vec4, y));
    engine->RegisterObjectProperty("vec4", "float z", asOFFSET(glm::vec4, z));
    engine->RegisterObjectProperty("vec4", "float w", asOFFSET(glm::vec4, w));
    engine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(vec4Constructor), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec4", "vec4 opAdd(const vec4 &in) const", asFUNCTIONPR(glmAdd, (const glm::vec4&, const void*), glm::vec4), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec4", "vec4 opSub(const vec4 &in) const", asFUNCTIONPR(glmSub, (const glm::vec4&, const void*), glm::vec4), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec4", "vec4 opMul(float) const", asFUNCTIONPR(glmMul, (float, const void*), glm::vec4), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec4", "vec4 opMul_r(float) const", asFUNCTIONPR(glmMulR, (float, const void*), glm::vec4), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec4", "vec4 opDiv(float) const", asFUNCTIONPR(glmDiv, (float, const void*), glm::vec4), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec4", "vec4 opDiv_r(float) const", asFUNCTIONPR(glmDivR, (float, const void*), glm::vec4), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("vec4", "vec4& opAddAssign(const vec4 &in) const", asMETHODPR(glm::vec4, operator+=, (const glm::vec4&), glm::vec4&), asCALL_THISCALL);
    engine->RegisterObjectMethod("vec4", "vec4& opSubAssign(const vec4 &in) const", asMETHODPR(glm::vec4, operator-=, (const glm::vec4&), glm::vec4&), asCALL_THISCALL);
    engine->RegisterObjectMethod("vec4", "vec4& opMulAssign(float) const", asMETHODPR(glm::vec4, operator*=, (float), glm::vec4&), asCALL_THISCALL);
    engine->RegisterObjectMethod("vec4", "vec4& opDivAssign(float) const", asMETHODPR(glm::vec4, operator/=, (float), glm::vec4&), asCALL_THISCALL);
    engine->RegisterObjectMethod("vec4", "vec4 opNeg() const", asFUNCTIONPR(glmNeg, (const void*), glm::vec4), asCALL_CDECL_OBJLAST);
    
    engine->RegisterObjectType("mat3", sizeof(glm::mat3), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<glm::mat3>());
    engine->RegisterObjectBehaviour("mat3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(glmConstructor<glm::mat3>, (void*), void), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("mat3", "mat3 opAdd(const mat3 &in) const", asFUNCTIONPR(glmAdd, (const glm::mat3&, const void*), glm::mat3), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("mat3", "mat3 opSub(const mat3 &in) const", asFUNCTIONPR(glmSub, (const glm::mat3&, const void*), glm::mat3), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("mat3", "mat3 opMul(float) const", asFUNCTIONPR(glmMul, (float, const void*), glm::mat3), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("mat3", "mat3 opMul_r(float) const", asFUNCTIONPR(glmMulR, (float, const void*), glm::mat3), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("mat3", "mat3 opMul(const mat3 &in) const", asFUNCTIONPR(glmMul, (const glm::mat3&, const void*), glm::mat3), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("mat3", "mat3 opDiv(float) const", asFUNCTIONPR(glmDiv, (float, const void*), glm::mat3), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("mat3", "mat3 opDiv_r(float) const", asFUNCTIONPR(glmDivR, (float, const void*), glm::mat3), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("mat3", "mat3& opAddAssign(const mat3 &in) const", asMETHODPR(glm::mat3, operator+=, (const glm::mat3&), glm::mat3&), asCALL_THISCALL);
    engine->RegisterObjectMethod("mat3", "mat3& opSubAssign(const mat3 &in) const", asMETHODPR(glm::mat3, operator-=, (const glm::mat3&), glm::mat3&), asCALL_THISCALL);
    engine->RegisterObjectMethod("mat3", "mat3& opMulAssign(float) const", asMETHODPR(glm::mat3, operator*=, (float), glm::mat3&), asCALL_THISCALL);
    engine->RegisterObjectMethod("mat3", "mat3& opDivAssign(float) const", asMETHODPR(glm::mat3, operator/=, (float), glm::mat3&), asCALL_THISCALL);
    engine->RegisterObjectMethod("mat3", "mat3 opNeg() const", asFUNCTIONPR(glmNeg, (const void*), glm::mat3), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("mat3", "vec3 opMul(const vec3 &in) const", asFUNCTION(mat3MulVec3), asCALL_CDECL_OBJLAST);
    
    engine->RegisterObjectType("mat4", sizeof(glm::mat4), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<glm::mat4>());
    engine->RegisterObjectBehaviour("mat4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(glmConstructor<glm::mat4>, (void*), void), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("mat4", "mat4 opAdd(const mat4 &in) const", asFUNCTIONPR(glmAdd, (const glm::mat4&, const void*), glm::mat4), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("mat4", "mat4 opSub(const mat4 &in) const", asFUNCTIONPR(glmSub, (const glm::mat4&, const void*), glm::mat4), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("mat4", "mat4 opMul(float) const", asFUNCTIONPR(glmMul, (float, const void*), glm::mat4), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("mat4", "mat4 opMul_r(float) const", asFUNCTIONPR(glmMulR, (float, const void*), glm::mat4), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("mat4", "mat4 opMul(const mat4 &in) const", asFUNCTIONPR(glmMul, (const glm::mat4&, const void*), glm::mat4), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("mat4", "mat4 opDiv(float) const", asFUNCTIONPR(glmDiv, (float, const void*), glm::mat4), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("mat4", "mat4 opDiv_r(float) const", asFUNCTIONPR(glmDivR, (float, const void*), glm::mat4), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("mat4", "mat4& opAddAssign(const mat4 &in) const", asMETHODPR(glm::mat4, operator+=, (const glm::mat4&), glm::mat4&), asCALL_THISCALL);
    engine->RegisterObjectMethod("mat4", "mat4& opSubAssign(const mat4 &in) const", asMETHODPR(glm::mat4, operator-=, (const glm::mat4&), glm::mat4&), asCALL_THISCALL);
    engine->RegisterObjectMethod("mat4", "mat4& opMulAssign(float) const", asMETHODPR(glm::mat4, operator*=, (float), glm::mat4&), asCALL_THISCALL);
    engine->RegisterObjectMethod("mat4", "mat4& opDivAssign(float) const", asMETHODPR(glm::mat4, operator/=, (float), glm::mat4&), asCALL_THISCALL);
    engine->RegisterObjectMethod("mat4", "mat4 opNeg() const", asFUNCTIONPR(glmNeg, (const void*), glm::mat4), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("mat4", "vec4 opMul(const vec4 &in) const", asFUNCTION(mat4MulVec4), asCALL_CDECL_OBJLAST);

    engine->RegisterObjectType("quat", sizeof(glm::quat), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<glm::quat>());
    engine->RegisterObjectBehaviour("quat", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(glmConstructor<glm::quat>, (void*), void), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("quat", "quat opAdd(const quat &in) const", asFUNCTIONPR(glmAdd, (const glm::quat&, const void*), glm::quat), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("quat", "quat opMul(float) const", asFUNCTIONPR(glmMul, (float, const void*), glm::quat), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("quat", "quat opMul_r(float) const", asFUNCTIONPR(glmMulR, (float, const void*), glm::quat), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("quat", "quat opMul(const quat &in) const", asFUNCTIONPR(glmMul, (const glm::quat&, const void*), glm::quat), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("quat", "quat& opMulAssign(float) const", asMETHODPR(glm::quat, operator*=, (float), glm::quat&), asCALL_THISCALL);
    engine->RegisterObjectMethod("quat", "quat opNeg() const", asFUNCTIONPR(glmNeg, (const void*), glm::quat), asCALL_CDECL_OBJLAST);
    
    // Register GLM functions.
    engine->RegisterGlobalFunction("vec2 normalize(const vec2 &in)", asFUNCTIONPR(glm::normalize, (const glm::vec2&), glm::vec2), asCALL_CDECL);
    engine->RegisterGlobalFunction("vec3 normalize(const vec3 &in)", asFUNCTIONPR(glm::normalize, (const glm::vec3&), glm::vec3), asCALL_CDECL);
    engine->RegisterGlobalFunction("vec4 normalize(const vec4 &in)", asFUNCTIONPR(glm::normalize, (const glm::vec4&), glm::vec4), asCALL_CDECL);
    engine->RegisterGlobalFunction("float length(const vec2 &in)", asFUNCTIONPR(glm::length, (const glm::vec2&), float), asCALL_CDECL);
    engine->RegisterGlobalFunction("float length(const vec3 &in)", asFUNCTIONPR(glm::length, (const glm::vec3&), float), asCALL_CDECL);
    engine->RegisterGlobalFunction("float length(const vec4 &in)", asFUNCTIONPR(glm::length, (const glm::vec4&), float), asCALL_CDECL);
    engine->RegisterGlobalFunction("vec3 cross(const vec3 &in, const vec3 &in)", asFUNCTIONPR(glm::cross, (const glm::vec3&, const glm::vec3&), glm::vec3), asCALL_CDECL);
    engine->RegisterGlobalFunction("float dot(const vec2 &in, const vec2 &in)", asFUNCTIONPR(glm::dot, (const glm::vec2&, const glm::vec2&), float), asCALL_CDECL);
    engine->RegisterGlobalFunction("float dot(const vec3 &in, const vec3 &in)", asFUNCTIONPR(glm::dot, (const glm::vec3&, const glm::vec3&), float), asCALL_CDECL);
    engine->RegisterGlobalFunction("float dot(const vec4 &in, const vec4 &in)", asFUNCTIONPR(glm::dot, (const glm::vec4&, const glm::vec4&), float), asCALL_CDECL);
    engine->RegisterGlobalFunction("float distance(const vec2 &in, const vec2 &in)", asFUNCTIONPR(glm::distance, (const glm::vec2&, const glm::vec2&), float), asCALL_CDECL);
    engine->RegisterGlobalFunction("float distance(const vec3 &in, const vec3 &in)", asFUNCTIONPR(glm::distance, (const glm::vec3&, const glm::vec3&), float), asCALL_CDECL);
    engine->RegisterGlobalFunction("float distance(const vec4 &in, const vec4 &in)", asFUNCTIONPR(glm::distance, (const glm::vec4&, const glm::vec4&), float), asCALL_CDECL);
    engine->RegisterGlobalFunction("mat3 inverse(const mat3 &in)", asFUNCTIONPR(glm::inverse, (const glm::mat3&), glm::mat3), asCALL_CDECL);
    engine->RegisterGlobalFunction("mat4 inverse(const mat4 &in)", asFUNCTIONPR(glm::inverse, (const glm::mat4&), glm::mat4), asCALL_CDECL);
    engine->RegisterGlobalFunction("mat3 transpose(const mat3 &in)", asFUNCTIONPR(glm::transpose, (const glm::mat3&), glm::mat3), asCALL_CDECL);
    engine->RegisterGlobalFunction("mat4 transpose(const mat4 &in)", asFUNCTIONPR(glm::transpose, (const glm::mat4&), glm::mat4), asCALL_CDECL);
    engine->RegisterGlobalFunction("float determinant(const mat3 &in)", asFUNCTIONPR(glm::determinant, (const glm::mat3&), float), asCALL_CDECL);
    engine->RegisterGlobalFunction("float determinant(const mat4 &in)", asFUNCTIONPR(glm::determinant, (const glm::mat4&), float), asCALL_CDECL);
    engine->RegisterGlobalFunction("float pitch(const quat &in)", asFUNCTIONPR(glm::pitch, (const glm::quat&), float), asCALL_CDECL);
    engine->RegisterGlobalFunction("float yaw(const quat &in)", asFUNCTIONPR(glm::yaw, (const glm::quat&), float), asCALL_CDECL);
    engine->RegisterGlobalFunction("float roll(const quat &in)", asFUNCTIONPR(glm::roll, (const glm::quat&), float), asCALL_CDECL);
    engine->RegisterGlobalFunction("float radians(float)", asFUNCTIONPR(glm::radians, (float), float), asCALL_CDECL);

    // Register Entity.
    engine->RegisterObjectType("Entity", 0, asOBJ_REF | asOBJ_NOCOUNT);
    engine->RegisterObjectProperty("Entity", "string name", asOFFSET(Entity, name));
    engine->RegisterObjectProperty("Entity", "quat rotation", asOFFSET(Entity, rotation));
    engine->RegisterObjectProperty("Entity", "vec3 position", asOFFSET(Entity, position));
    engine->RegisterObjectProperty("Entity", "vec3 scale", asOFFSET(Entity, scale));
    engine->RegisterObjectMethod("Entity", "vec3 GetWorldPosition() const", asMETHOD(Entity, GetWorldPosition), asCALL_THISCALL);
    engine->RegisterObjectMethod("Entity", "void Kill()", asMETHOD(Entity, Kill), asCALL_THISCALL);
    engine->RegisterObjectMethod("Entity", "bool IsKilled() const", asMETHOD(Entity, IsKilled), asCALL_THISCALL);
    engine->RegisterObjectMethod("Entity", "Entity@ GetParent() const", asMETHOD(Entity, GetParent), asCALL_THISCALL);
    engine->RegisterObjectMethod("Entity", "Entity@ InstantiateScene(const string &in)", asMETHOD(Entity, InstantiateScene), asCALL_THISCALL);
    engine->RegisterObjectMethod("Entity", "bool IsScene() const", asMETHOD(Entity, IsScene), asCALL_THISCALL);
    engine->RegisterObjectMethod("Entity", "Entity@ GetChild(const string &in) const", asMETHOD(Entity, GetChild), asCALL_THISCALL);

    engine->RegisterGlobalFunction("Entity@ GetEntity(uint GUID)", asFUNCTIONPR(ScriptManager::GetEntity, (unsigned int), Entity*), asCALL_CDECL);

    engine->RegisterObjectMethod("Entity", "void RotateYaw(float angle)", asMETHOD(Entity, RotateYaw), asCALL_THISCALL);
    engine->RegisterObjectMethod("Entity", "void RotatePitch(float angle)", asMETHOD(Entity, RotatePitch), asCALL_THISCALL);
    engine->RegisterObjectMethod("Entity", "void RotateRoll(float angle)", asMETHOD(Entity, RotateRoll), asCALL_THISCALL);
    engine->RegisterObjectMethod("Entity", "void RotateAroundWorldAxis(float, const vec3 &in)", asMETHOD(Entity, RotateAroundWorldAxis), asCALL_THISCALL);
    engine->RegisterObjectMethod("Entity", "void SetWorldOrientation(quat)", asMETHOD(Entity, SetWorldOrientation), asCALL_THISCALL);
    engine->RegisterObjectMethod("Entity", "void SetLocalOrientation(quat)", asMETHOD(Entity, SetLocalOrientation), asCALL_THISCALL);
    
    // Register components.
    engine->SetDefaultNamespace("Component");
    
//   engine->RegisterObjectType("AnimationController", 0, asOBJ_REF | asOBJ_NOCOUNT);
//   engine->RegisterObjectMethod("AnimationController", "void SetBool(string name, bool value)", asMETHOD(AnimationController, SetBool), asCALL_THISCALL);
//   engine->RegisterObjectMethod("AnimationController", "void SetFloat(string name, float value)", asMETHOD(AnimationController, SetFloat), asCALL_THISCALL);

    engine->RegisterObjectType("DirectionalLight", 0, asOBJ_REF | asOBJ_NOCOUNT);
    engine->RegisterObjectProperty("DirectionalLight", "vec3 color", asOFFSET(DirectionalLight, color));
    engine->RegisterObjectProperty("DirectionalLight", "float ambientCoefficient", asOFFSET(DirectionalLight, ambientCoefficient));
    
    engine->RegisterObjectType("Lens", 0, asOBJ_REF | asOBJ_NOCOUNT);
    engine->RegisterObjectProperty("Lens", "float fieldOfView", asOFFSET(Lens, fieldOfView));
    engine->RegisterObjectProperty("Lens", "float zNear", asOFFSET(Lens, zNear));
    engine->RegisterObjectProperty("Lens", "float zFar", asOFFSET(Lens, zFar));
    
    engine->RegisterObjectType("Listener", 0, asOBJ_REF | asOBJ_NOCOUNT);
    
    engine->RegisterObjectType("PointLight", 0, asOBJ_REF | asOBJ_NOCOUNT);
    engine->RegisterObjectProperty("PointLight", "vec3 color", asOFFSET(PointLight, color));
    engine->RegisterObjectProperty("PointLight", "float attenuation", asOFFSET(PointLight, attenuation));
    engine->RegisterObjectProperty("PointLight", "float intensity", asOFFSET(PointLight, intensity));

    engine->RegisterObjectType("RigidBody", 0, asOBJ_REF | asOBJ_NOCOUNT);

    engine->RegisterObjectType("SpotLight", 0, asOBJ_REF | asOBJ_NOCOUNT);
    engine->RegisterObjectProperty("SpotLight", "vec3 color", asOFFSET(SpotLight, color));
    engine->RegisterObjectProperty("SpotLight", "float ambientCoefficient", asOFFSET(SpotLight, ambientCoefficient));
    engine->RegisterObjectProperty("SpotLight", "float attenuation", asOFFSET(SpotLight, attenuation));
    engine->RegisterObjectProperty("SpotLight", "float intensity", asOFFSET(SpotLight, intensity));
    engine->RegisterObjectProperty("SpotLight", "float coneAngle", asOFFSET(SpotLight, coneAngle));
    
    engine->RegisterObjectType("SoundSource", 0, asOBJ_REF | asOBJ_NOCOUNT);
    engine->RegisterObjectProperty("SoundSource", "float volume", asOFFSET(SoundSource, volume));
    engine->RegisterObjectProperty("SoundSource", "bool loop", asOFFSET(SoundSource, loop));
    engine->RegisterObjectMethod("SoundSource", "void Play()", asMETHOD(SoundSource, Play), asCALL_THISCALL);
    engine->RegisterObjectMethod("SoundSource", "void Pause()", asMETHOD(SoundSource, Pause), asCALL_THISCALL);
    engine->RegisterObjectMethod("SoundSource", "void Stop()", asMETHOD(SoundSource, Stop), asCALL_THISCALL);
    
    engine->SetDefaultNamespace("");
    
    // Register getting components.
    engine->RegisterObjectMethod("Entity", "Component::DirectionalLight@ GetDirectionalLight()", asMETHODPR(Entity, GetComponent, () const, DirectionalLight*), asCALL_THISCALL);
    engine->RegisterObjectMethod("Entity", "Component::Lens@ GetLens()", asMETHODPR(Entity, GetComponent, () const, Lens*), asCALL_THISCALL);
    engine->RegisterObjectMethod("Entity", "Component::Listener@ GetListener()", asMETHODPR(Entity, GetComponent, () const, Listener*), asCALL_THISCALL);
    engine->RegisterObjectMethod("Entity", "Component::PointLight@ GetPointLight()", asMETHODPR(Entity, GetComponent, () const, PointLight*), asCALL_THISCALL);
    engine->RegisterObjectMethod("Entity", "Component::RigidBody@ GetRigidBody()", asMETHODPR(Entity, GetComponent, () const, RigidBody*), asCALL_THISCALL);
    engine->RegisterObjectMethod("Entity", "Component::SpotLight@ GetSpotLight()", asMETHODPR(Entity, GetComponent, () const, SpotLight*), asCALL_THISCALL);
    engine->RegisterObjectMethod("Entity", "Component::SoundSource@ GetSoundSource()", asMETHODPR(Entity, GetComponent, () const, SoundSource*), asCALL_THISCALL);
    
    // Register managers.
    engine->RegisterObjectType("DebugDrawingManager", 0, asOBJ_REF | asOBJ_NOCOUNT);
    engine->RegisterObjectMethod("DebugDrawingManager", "void AddPoint(const vec3 &in, const vec3 &in, float, float = 0.0, bool = true)", asMETHOD(DebugDrawingManager, AddPoint), asCALL_THISCALL);
    engine->RegisterObjectMethod("DebugDrawingManager", "void AddLine(const vec3 &in, const vec3 &in, const vec3 &in, float = 1.0, float = 0.0, bool = true)", asMETHOD(DebugDrawingManager, AddLine), asCALL_THISCALL);
    engine->RegisterObjectMethod("DebugDrawingManager", "void AddCuboid(const vec3 &in, const mat4 &in, const vec3 &in, float = 1.0, float = 0.0, bool = true)", asMETHOD(DebugDrawingManager, AddCuboid), asCALL_THISCALL);
    engine->RegisterObjectMethod("DebugDrawingManager", "void AddPlane(const vec3 &in, const vec3 &in, const vec2 &in, const vec3 &in, float = 1.0, float = 0.0, bool = true)", asMETHOD(DebugDrawingManager, AddPlane), asCALL_THISCALL);
    engine->RegisterObjectMethod("DebugDrawingManager", "void AddSphere(const vec3 &in, float, const vec3 &in, float = 1.0, float = 0.0, bool = true)", asMETHOD(DebugDrawingManager, AddSphere), asCALL_THISCALL);
    engine->RegisterObjectMethod("DebugDrawingManager", "void AddCylinder(float, float, const mat4& in, const vec3 &in, float = 1.0, float = 0.0, bool = true)", asMETHOD(DebugDrawingManager, AddCylinder), asCALL_THISCALL);
    engine->RegisterObjectMethod("DebugDrawingManager", "void AddCone(float, float, const mat4& in, const vec3 &in, float = 1.0, float = 0.0, bool = true)", asMETHOD(DebugDrawingManager, AddCone), asCALL_THISCALL);

    engine->RegisterObjectType("RenderManager", 0, asOBJ_REF | asOBJ_NOCOUNT);
    engine->RegisterObjectMethod("RenderManager", "void SetGamma(float)", asMETHOD(RenderManager, SetGamma), asCALL_THISCALL);
    engine->RegisterObjectMethod("RenderManager", "float GetGamma()", asMETHOD(RenderManager, GetGamma), asCALL_THISCALL);
    engine->RegisterObjectMethod("RenderManager", "void SetFogApply(bool)", asMETHOD(RenderManager, SetFogApply), asCALL_THISCALL);
    engine->RegisterObjectMethod("RenderManager", "bool GetFogApply()", asMETHOD(RenderManager, GetFogApply), asCALL_THISCALL);
    engine->RegisterObjectMethod("RenderManager", "void SetFogDensity(float)", asMETHOD(RenderManager, SetFogDensity), asCALL_THISCALL);
    engine->RegisterObjectMethod("RenderManager", "float GetFogDensity()", asMETHOD(RenderManager, GetFogDensity), asCALL_THISCALL);
    engine->RegisterObjectMethod("RenderManager", "void SetFogColor(const vec3 &in)", asMETHOD(RenderManager, SetFogColor), asCALL_THISCALL);
    engine->RegisterObjectMethod("RenderManager", "vec3 GetFogColor()", asMETHOD(RenderManager, GetFogColor), asCALL_THISCALL);
    engine->RegisterObjectMethod("RenderManager", "void SetColorFilterApply(bool)", asMETHOD(RenderManager, SetColorFilterApply), asCALL_THISCALL);
    engine->RegisterObjectMethod("RenderManager", "vec3 GetColorFilterColor()", asMETHOD(RenderManager, GetColorFilterColor), asCALL_THISCALL);
    engine->RegisterObjectMethod("RenderManager", "void SetDitherApply(bool)", asMETHOD(RenderManager, SetDitherApply), asCALL_THISCALL);
    engine->RegisterObjectMethod("RenderManager", "bool GetDitherApply()", asMETHOD(RenderManager, GetDitherApply), asCALL_THISCALL);

    engine->RegisterObjectType("PhysicsManager", 0, asOBJ_REF | asOBJ_NOCOUNT);
    engine->RegisterObjectMethod("PhysicsManager", "void MakeKinematic(Component::RigidBody@)", asMETHOD(PhysicsManager, MakeKinematic), asCALL_THISCALL);
    engine->RegisterObjectMethod("PhysicsManager", "void MakeDynamic(Component::RigidBody@)", asMETHOD(PhysicsManager, MakeDynamic), asCALL_THISCALL);
    engine->RegisterObjectMethod("PhysicsManager", "void ForceTransformSync(Component::RigidBody@)", asMETHOD(PhysicsManager, ForceTransformSync), asCALL_THISCALL);
    engine->RegisterObjectMethod("PhysicsManager", "void HaltMovement(Component::RigidBody@)", asMETHOD(PhysicsManager, HaltMovement), asCALL_THISCALL);

    engine->RegisterObjectType("Hub", 0, asOBJ_REF | asOBJ_NOCOUNT);
    engine->RegisterObjectProperty("Hub", "DebugDrawingManager@ debugDrawingManager", asOFFSET(Hub, debugDrawingManager));
    engine->RegisterObjectProperty("Hub", "RenderManager@ renderManager", asOFFSET(Hub, renderManager));
    engine->RegisterObjectProperty("Hub", "PhysicsManager@ physicsManager", asOFFSET(Hub, physicsManager));

    // Register functions.
    engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL);
    engine->RegisterGlobalFunction("void RestartScene()", asFUNCTION(RestartScene), asCALL_CDECL);
    engine->RegisterGlobalFunction("void RegisterUpdate()", asFUNCTION(::RegisterUpdate), asCALL_CDECL);
    engine->RegisterGlobalFunction("void RegisterTriggerEnter(Component::RigidBody@, Component::RigidBody@, const string &in)", asFUNCTION(RegisterTriggerEnterHelper), asCALL_CDECL);
    engine->RegisterGlobalFunction("void RegisterTriggerRetain(Component::RigidBody@, Component::RigidBody@, const string &in)", asFUNCTION(RegisterTriggerRetainHelper), asCALL_CDECL);
    engine->RegisterGlobalFunction("void RegisterTriggerLeave(Component::RigidBody@, Component::RigidBody@, const string &in)", asFUNCTION(RegisterTriggerLeaveHelper), asCALL_CDECL);
    engine->RegisterGlobalFunction("bool Input(input button)", asFUNCTION(ButtonInput), asCALL_CDECL);
    engine->RegisterGlobalFunction("void SendMessage(Entity@, int)", asFUNCTION(::SendMessage), asCALL_CDECL);
    engine->RegisterGlobalFunction("Hub@ Managers()", asFUNCTION(Managers), asCALL_CDECL);
    engine->RegisterGlobalFunction("vec2 GetCursorXY()", asFUNCTION(GetCursorXY), asCALL_CDECL);
    engine->RegisterGlobalFunction("bool IsIntersect(Entity@, Entity@)", asFUNCTION(IsIntersect), asCALL_CDECL);
    engine->RegisterGlobalFunction("bool IsVRActive()", asFUNCTION(IsVRActive), asCALL_CDECL);
}

ScriptManager::~ScriptManager() {
    engine->ShutDownAndRelease();
}

int ScriptManager::BuildScript(ScriptFile* script) {

    GetBreakpoints(script);

    std::string filename = Hymn().GetPath() + "/" + script->path + script->name + ".as";
    if (!FileSystem::FileExists(filename.c_str())) {
        Log() << "Script file does not exist: " << filename << "\n";
        return -1;
    }
    
    // Create and build script module.
    CScriptBuilder builder;
    int r = builder.StartNewModule(engine, script->name.c_str());
    if (r < 0) {
        Log() << "Couldn't start new module: " << script->name << ".\n";
        return r;
    }
    
    r = builder.AddSectionFromFile(filename.c_str());
    if (r < 0) {
        Log() << "File section could not be added: " << filename << ".\n";
        return r;
    }
    
    r = builder.BuildModule();
    if (r < 0) {
        Log() << "Compile errors.\n";
        return r;
    }

    FillFunctionVector(script);

    return r;

}

void ScriptManager::BuildAllScripts() {

    std::string path = Hymn().GetPath() + "/";
    
    for (ScriptFile* file : Hymn().scripts) {
        std::string filename = path + file->path + file->name + ".as";
        if (!FileSystem::FileExists(filename.c_str())) {
            Log() << "Script file does not exist: " << filename << "\n";
            return;
        }

        // We get the breakpoints.
        GetBreakpoints(file);
        
        // Create and build script module.
        CScriptBuilder builder;
        asIScriptModule* module = engine->GetModule(file->name.c_str());
        if (module == nullptr) {
            int r = builder.StartNewModule(engine, file->name.c_str());
            if (r < 0)
                Log() << "Couldn't start new module: " << file->name << ".\n";
            
            r = builder.AddSectionFromFile(filename.c_str());
            if (r < 0)
                Log() << "File section could not be added: " << filename << ".\n";

            r = builder.BuildModule();
            if (r < 0)
                Log() << "Compile errors.\n";

        } else {
            std::string script;

            LoadScriptFile(filename.c_str(), script);
            module->AddScriptSection(filename.c_str(), script.c_str());
            
            int r = module->Build();
            if (r < 0)
                Log() << file->name.c_str() << "Compile errors.\n";
        }

        FillFunctionVector(file);

    }
}

void ScriptManager::GetBreakpoints(const ScriptFile* scriptFile) {

    //If we already fetched the breakpoints for this file, we clear it.
    auto it = breakpoints.find(scriptFile->name + ".as");
    if (it != breakpoints.end())
        breakpoints[scriptFile->name + ".as"].clear();

    std::string path = Hymn().GetPath() + "/";
    std::string filePath = path + scriptFile->path + scriptFile->name + ".as";

    std::string scriptLines;
    LoadScriptFile(filePath.c_str(), scriptLines);
    
    std::istringstream f(scriptLines);
    std::string line;
    int lineNumber = 1;
    while (std::getline(f, line)) {
        if (line.length() >= 7) {

            std::string end = line.substr(line.length() - 8, 7);
            if (end == "//break" || end == "//Break" || end == "//BREAK") {

                breakpoints[scriptFile->name + ".as"].insert(lineNumber);

            }
        }
        lineNumber++;
    }
}

void ScriptManager::ClearBreakpoints() {

    for (auto pair : breakpoints)
        pair.second.clear();

    breakpoints.clear();

}

void ScriptManager::FillPropertyMap(Script* script) {

    int r = BuildScript(script->scriptFile);
    if (r < 0) {

        Log() << "Couldn't fetch properties" << "\n";

    } else {

        CreateInstance(script);

        int propertyCount = script->instance->GetPropertyCount();

        for (int n = 0; n < propertyCount; n++) {

            std::string name = script->instance->GetPropertyName(n);
            int typeId = script->instance->GetPropertyTypeId(n);
            void* varPointer = script->instance->GetAddressOfProperty(n);

            if (script->IsInPropertyMap(name, typeId))
                continue;

            if (typeId == asTYPEID_INT32) {
                int size = sizeof(int);
                script->AddToPropertyMap(name, typeId, size, varPointer);
            } else if (typeId == asTYPEID_FLOAT) {
                int size = sizeof(float);
                script->AddToPropertyMap(name, typeId, size, varPointer);
            } else if (typeId == engine->GetTypeIdByDecl("Entity@")) {
                int size = sizeof(unsigned int);
                
                const std::vector<Entity*> entities = Hymn().world.GetEntities();
                
                unsigned int GUID = 0;
                if (entities.size() != 0)
                    GUID = 0;
                else 
                    GUID = entities[0]->GetUniqueIdentifier();
                
                script->AddToPropertyMap(name, typeId, size, (void*)(&GUID));

            }

        }

    }

}

void ScriptManager::FillFunctionVector(ScriptFile* scriptFile) {

    scriptFile->functionList.clear();

    asITypeInfo* scriptClass = GetClass(scriptFile->name, scriptFile->name);
    int functionCount = scriptClass->GetMethodCount();
    for (int n = 0; n < functionCount; n++) {

        asIScriptFunction* func = scriptClass->GetMethodByIndex(n);
        std::string decl = func->GetDeclaration(false);

        scriptFile->functionList.push_back(decl);

    }

}


void ScriptManager::Update(World& world, float deltaTime) {
    // Init.
    for (Script* script : scripts.GetAll()) {
        if (!script->initialized && !script->IsKilled() && script->entity->enabled) {
            CreateInstance(script);

            // Skip if not initialized
            if (!script->initialized)
                continue;

            int propertyCount = script->instance->GetPropertyCount();

            for (int n = 0; n < propertyCount; n++) {

                std::string name = script->instance->GetPropertyName(n);
                int typeId = script->instance->GetPropertyTypeId(n);
                void *varPointer = script->instance->GetAddressOfProperty(n);

                if (script->IsInPropertyMap(name, typeId)) {

                    if (typeId == engine->GetTypeIdByDecl("Entity@"))
                        *reinterpret_cast<Entity*>(varPointer) = *GetEntity(*(unsigned int*)script->GetDataFromPropertyMap(name));
                    else 
                        script->CopyDataFromPropertyMap(name, varPointer);

                } 
            }
        }
    }
    
    // Update.
    for (Entity* entity : world.GetUpdateEntities()) {
        this->currentEntity = entity;
        CallUpdate(entity, deltaTime);
    }
    
    // Handle messages.
    while (!messages.empty()) {
        std::vector<Message> temp = messages;
        messages.clear();
        
        for (const Message& message : temp)
            CallMessageReceived(message);
    }
    
    // Register entities for events.
    for (Entity* entity : updateEntities)
        world.RegisterUpdate(entity);
    updateEntities.clear();
    
    // Handle physics triggers.
    for (const TriggerEvent& triggerEvent : triggerEvents) {
        CallTrigger(triggerEvent);
    }
    triggerEvents.clear();
}

void ScriptManager::RegisterUpdate(Entity* entity) {
    updateEntities.push_back(entity);
}

void ScriptManager::RegisterTriggerEnter(Entity* entity, Component::RigidBody* trigger, Component::RigidBody* object, const std::string& methodName) {
    TriggerEvent triggerEvent;
    triggerEvent.trigger = trigger;
    triggerEvent.object = object;
    triggerEvent.scriptEntity = entity;
    triggerEvent.methodName = methodName;
    
    Managers().physicsManager->OnTriggerEnter(trigger, object, std::bind(&ScriptManager::HandleTrigger, this, triggerEvent));
}

void ScriptManager::RegisterTriggerRetain(Entity* entity, Component::RigidBody* trigger, Component::RigidBody* object, const std::string& methodName) {
    TriggerEvent triggerEvent;
    triggerEvent.trigger = trigger;
    triggerEvent.object = object;
    triggerEvent.scriptEntity = entity;
    triggerEvent.methodName = methodName;

    Managers().physicsManager->OnTriggerRetain(trigger, object, std::bind(&ScriptManager::HandleTrigger, this, triggerEvent));
}

void ScriptManager::RegisterTriggerLeave(Entity* entity, Component::RigidBody* trigger, Component::RigidBody* object, const std::string& methodName) {
    TriggerEvent triggerEvent;
    triggerEvent.trigger = trigger;
    triggerEvent.object = object;
    triggerEvent.scriptEntity = entity;
    triggerEvent.methodName = methodName;

    Managers().physicsManager->OnTriggerLeave(trigger, object, std::bind(&ScriptManager::HandleTrigger, this, triggerEvent));
}

void ScriptManager::RegisterInput() {
    // Get the input enum.
    asUINT enumCount = engine->GetEnumCount();
    asITypeInfo* inputEnum = nullptr;
    for (asUINT i = 0; i < enumCount; ++i) {
        asITypeInfo* asEnum = engine->GetEnumByIndex(i);
        std::string name = asEnum->GetName();
        if (name == "input") {
            inputEnum = asEnum;
            break;
        }
    }
    
    for (std::size_t i = 0; i < Input::GetInstance().buttons.size(); ++i) {
        Input::Button* button = Input::GetInstance().buttons[i];
        
        // Check if we've already registered the button.
        bool registered = false;
        asUINT inputCount = inputEnum->GetEnumValueCount();
        for (asUINT j = 0; j < inputCount; ++j) {
            int value;
            std::string registeredButton = inputEnum->GetEnumValueByIndex(j, &value);
            if (registeredButton == button->action) {
                registered = true;
                break;
            }
        }
        
        if (!registered)
            engine->RegisterEnumValue("input", std::string(button->action).c_str(), i);
    }
}

void ScriptManager::SendMessage(Entity* recipient, int type) {
    Message message;
    message.recipient = recipient;
    message.type = type;
    messages.push_back(message);
}

Entity* ScriptManager::GetEntity(unsigned int GUID) {

    const std::vector<Entity*> entities = Hymn().world.GetEntities();
    for (std::size_t i = 0; i < entities.size(); ++i) {

        if (entities[i]->GetUniqueIdentifier() == GUID) {

            return entities[i];

        }

    }

    return nullptr;

}

Component::Script* ScriptManager::CreateScript() {
    return scripts.Create();
}

Component::Script* ScriptManager::CreateScript(const Json::Value& node) {
    Component::Script* script = scripts.Create();
    
    // Load values from Json node.
    std::string name = node.get("scriptName", "").asString();
    script->scriptFile = Managers().resourceManager->CreateScriptFile(name);

    if (node.isMember("propertyMap")) {

        Json::Value propertyMapJson = node.get("propertyMap", "");
        std::vector<std::string> names = propertyMapJson.getMemberNames();

        for (auto& name : names) {

            if (propertyMapJson.isMember(name)) {

                Json::Value typeId_value = propertyMapJson.get(name, "");

                std::vector<std::string> typeIds = typeId_value.getMemberNames();
                int typeId = std::atoi(typeIds[0].c_str());
                int size = typeId_value[typeIds[0]].size();
                void* data = malloc(size + 1);
                for (int i = 0; i < size; i++)
                    ((unsigned char*)data)[i] = (unsigned char)(typeId_value[typeIds[0]][i].asInt());

                script->AddToPropertyMap(name, typeId, size, data);
                std::free(data);

            }
        }
    }
    
    return script;
}

int ScriptManager::GetStringDeclarationID() {

    return engine->GetTypeIdByDecl("string");

}

const std::vector<Component::Script*>& ScriptManager::GetScripts() const {
    return scripts.GetAll();
}

void ScriptManager::ClearKilledComponents() {
    scripts.ClearKilled();
}

void ScriptManager::ExecuteScriptMethod(const Entity* entity, const std::string& method) {
    Component::Script* script = entity->GetComponent<Component::Script>();
    if (!script)
        return;
    ScriptFile* scriptFile = script->scriptFile;

    // Get class.
    asITypeInfo* type = GetClass(scriptFile->name, scriptFile->name);

    // Find method to call.
    std::string methodDecl;
    methodDecl.reserve(method.length() + 7); // additional `void ` and `()`
    methodDecl.append("void ").append(method).append("()");
    asIScriptFunction* scriptMethod = type->GetMethodByDecl(methodDecl.c_str());
    if (scriptMethod == nullptr)
        Log() << "Can't find method void " << method << "()\n";

    // Create context, prepare it and execute.
    asIScriptContext* context = engine->CreateContext();
    context->Prepare(scriptMethod);
    context->SetObject(script->instance);
    ExecuteCall(context);

    // Clean up.
    context->Release();
}

void ScriptManager::CreateInstance(Component::Script* script) {
    currentEntity = script->entity;
    ScriptFile* scriptFile = script->scriptFile;

    // Skip if no script file.
    if (!scriptFile)
        return;
    
    // Find the class to instantiate.
    asITypeInfo* type = GetClass(scriptFile->name, scriptFile->name);
    
    // Skip if no class is found.
    if (!type)
        return;

    // Find factory function / constructor.
    std::string factoryName = scriptFile->name + "@ " + scriptFile->name + "(Entity@)";
    asIScriptFunction* factoryFunction = type->GetFactoryByDecl(factoryName.c_str());
    if (factoryFunction == nullptr)
        Log() << "Couldn't find the factory function for " << scriptFile->name << ".\n";
    
    // Create context, prepare it and execute.
    asIScriptContext* context = CreateContext();
    context->Prepare(factoryFunction);
    context->SetArgObject(0, script->entity);
    ExecuteCall(context);
    
    // Get the newly created object.
    script->instance = *(static_cast<asIScriptObject**>(context->GetAddressOfReturnValue()));
    script->instance->AddRef();

    // Clean up.
    context->Release();

    // Set initialized.
    script->initialized = true;
}

asIScriptContext* ScriptManager::CreateContext() {

    asIScriptContext* context = engine->CreateContext();
    context->SetLineCallback(asFUNCTION(AngelScriptDebugLineCallback), &breakpoints, asCALL_CDECL);
    return context;

}

void ScriptManager::CallMessageReceived(const Message& message) {
    currentEntity = message.recipient;
    Component::Script* script = currentEntity->GetComponent<Component::Script>();
    ScriptFile* scriptFile = script->scriptFile;
    
    // Get class.
    asITypeInfo* type = GetClass(scriptFile->name, scriptFile->name);
    
    // Find method to call.
    asIScriptFunction* method = type->GetMethodByDecl("void ReceiveMessage(int)");
    if (method == nullptr)
        Log() << "Can't find method void ReceiveMessage(int)\n";
    
    // Create context, prepare it and execute.
    asIScriptContext* context = CreateContext();
    context->Prepare(method);
    context->SetObject(script->instance);
    context->SetArgDWord(0, message.type);
    ExecuteCall(context);
    
    // Clean up.
    context->Release();
}

void ScriptManager::CallUpdate(Entity* entity, float deltaTime) {
    Component::Script* script = entity->GetComponent<Component::Script>();
    ScriptFile* scriptFile = script->scriptFile;
    
    // Get class.
    asITypeInfo* type = GetClass(scriptFile->name, scriptFile->name);
    
    // Find method to call.
    asIScriptFunction* method = type->GetMethodByDecl("void Update(float)");
    if (method == nullptr)
        Log() << "Can't find method void Update(float)\n";
    
    // Create context, prepare it and execute.
    asIScriptContext* context = CreateContext();
    context->Prepare(method);
    context->SetObject(script->instance);
    context->SetArgFloat(0, deltaTime);
    ExecuteCall(context);
    
    // Clean up.
    context->Release();
}

void ScriptManager::CallTrigger(const TriggerEvent& triggerEvent) {
    Component::Script* script = triggerEvent.scriptEntity->GetComponent<Component::Script>();
    ScriptFile* scriptFile = script->scriptFile;
    
    // Get class.
    asITypeInfo* type = GetClass(scriptFile->name, scriptFile->name);
    
    // Find method to call.
    std::string methodDeclaration = "void " + triggerEvent.methodName + "(Component::RigidBody@, Component::RigidBody@)";
    asIScriptFunction* method = type->GetMethodByDecl(methodDeclaration.c_str());
    if (method == nullptr)
        Log() << "Can't find method " << methodDeclaration << "\n";
    
    // Create context, prepare it and execute.
    asIScriptContext* context = CreateContext();
    context->Prepare(method);
    context->SetObject(script->instance);
    context->SetArgAddress(0, triggerEvent.trigger);
    context->SetArgAddress(1, triggerEvent.object);
    ExecuteCall(context);
    
    // Clean up.
    context->Release();
}

void ScriptManager::LoadScriptFile(const char* fileName, std::string& script){
    // Open the file in binary mode
    FILE* f = fopen(fileName, "rb");
    
    // Determine the size of the file
    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Load the entire file in one call
    script.resize(len);
    fread(&script[0], len, 1, f);
    
    fclose(f);
}

void ScriptManager::ExecuteCall(asIScriptContext* context) {
    int r = context->Execute();
    if (r != asEXECUTION_FINISHED) {
        // The execution didn't complete as expected. Determine what happened.
        if (r == asEXECUTION_EXCEPTION) {
            // An exception occurred, let the script writer know what happened so it can be corrected.
            Log() << "An exception '" << context->GetExceptionString() << "' occurred. Please correct the code and try again.\n";
        }
    }
}

asITypeInfo* ScriptManager::GetClass(const std::string& moduleName, const std::string& className) {
    // Get script module.
    asIScriptModule* module = engine->GetModule(moduleName.c_str(), asGM_ONLY_IF_EXISTS);
    if (module == nullptr) {
        Log() << "Couldn't find \"" << moduleName << "\" module.\n";
        return nullptr;
    }
    
    // Find the class.
    asUINT typeCount = module->GetObjectTypeCount();
    for (asUINT i = 0; i < typeCount; ++i) {
        asITypeInfo* type = module->GetObjectTypeByIndex(i);
        if (strcmp(type->GetName(), className.c_str()) == 0)
            return type;
    }
    
    Log() << "Couldn't find class \"" << className << "\".\n";
    return nullptr;
}

void ScriptManager::HandleTrigger(TriggerEvent triggerEvent) {
    triggerEvents.push_back(triggerEvent);
}
