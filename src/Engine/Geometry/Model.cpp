#include "Model.hpp"

#include "../Util/Log.hpp"
#include "../Hymn.hpp"
#include "../Util/FileSystem.hpp"
#include <assimp/cimport.h> // aiMultiplyMatrix4

using namespace Geometry;

Assimp::Importer Model::aiImporter = Assimp::Importer();

Model::Model() {

}

Model::Model(const char* filename) {
    Load(filename);
}

Geometry3D::Vertex* Model::GetVertices() const {
    return vertexData;
}

unsigned int Model::GetVertexCount() const {
    return vertexNr;
}

unsigned int* Model::GetIndices() const {
    return indexData;
}

unsigned int Model::GetIndexCount() const {
    return indexNr;
}

Json::Value Model::Save() const {
    Json::Value model;
    model["name"] = name;
    return model;
}

void Model::Load(const Json::Value& node) {
    name = node.get("name", "").asString();
    Load((Hymn().GetPath() + FileSystem::DELIMITER + "Models" + FileSystem::DELIMITER + name + ".fbx").c_str());
}

void Model::Load(const char* filename) {
    const aiScene* scene = aiImporter.ReadFile(filename, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | 0 );

    assert(scene->HasMeshes());
    if (scene->mNumMeshes > 1) {
        Log() << "Warning loading model: " << filename << " contains more then one mesh. Only one mesh will be loaded.\n";
    }

    Mesh mesh;
    mesh.assimpMesh = scene->mMeshes[0];
    mesh.rootNode = scene->mRootNode;
    LoadMesh(scene->mMeshes[0], mesh);
    TransfromMesh(mesh);



    //// http://gamedev.stackexchange.com/questions/26382/i-cant-figure-out-how-to-animate-my-loaded-model-with-assimp
    //// http://www.gamedev.net/topic/648859-assimp-build-the-skeleton-from-the-scene/

    // Importing the Animations
    if (scene->HasAnimations()) {
        const aiAnimation* animation = scene->mAnimations[0];
        float animtick = 2;
        float timestep = 1.f/60.f;
        float animfps = 1;
        animtick = animtick + (timestep / 1000.f) * animfps;
        AninmateMesh(animation, 1, mesh);
    }

    vertexData = mesh.vertexData;
    vertexNr = mesh.vertexNr;
    indexData = mesh.indexData;
    indexNr = mesh.indexNr;

    GenerateBuffers();
    GenerateVertexArray();
    //CreateAxisAlignedBoundingBox();

    mesh.Clear();

    aiImporter.FreeScene();
}

void Model::LoadMesh(const aiMesh* assimpMesh, Mesh& mesh) {
    mesh.vertexNr = assimpMesh->mNumVertices;
    mesh.vertexData = new Vertex[mesh.vertexNr];
    mesh.indexNr = assimpMesh->mNumFaces * 3;
    mesh.indexData = new unsigned int[mesh.indexNr];

    for (unsigned int i = 0; i < mesh.vertexNr; ++i) {
        CpyVec(assimpMesh->mVertices[i], mesh.vertexData[i].position);
        CpyVec(assimpMesh->mTextureCoords[0][i], mesh.vertexData[i].textureCoordinate);
        CpyVec(assimpMesh->mNormals[i], mesh.vertexData[i].normal);
        CpyVec(assimpMesh->mTangents[i], mesh.vertexData[i].tangent);
    }

    for (unsigned int i = 0; i < assimpMesh->mNumFaces; ++i) {
        mesh.indexData[i * 3] = assimpMesh->mFaces[i].mIndices[0];
        mesh.indexData[i * 3 + 1] = assimpMesh->mFaces[i].mIndices[1];
        mesh.indexData[i * 3 + 2] = assimpMesh->mFaces[i].mIndices[2];
    }
}

void Model::TransfromMesh(Mesh& mesh) {
    const aiMesh* assimpMesh = mesh.assimpMesh;
    if (!assimpMesh->HasBones())
        return;

    // Collapse mesh
    for (unsigned int i = 0; i < mesh.vertexNr; ++i) {
        mesh.vertexData[i].position = glm::vec3(0.f, 0.f, 0.f);
        mesh.vertexData[i].normal = glm::vec3(0.f, 0.f, 0.f);
    }

    aiMatrix4x4 skin4x4Mat;
    aiMatrix3x3 skin3x3Mat;

    for (unsigned int b = 0; b < assimpMesh->mNumBones; ++b) {
        const aiBone* bone = assimpMesh->mBones[b];
        const aiNode* node = FindNode(mesh.rootNode, bone->mName.data); // Find node corresponding to this bone

        TransformNode(node, skin4x4Mat); // Transform bone matrix
        aiMultiplyMatrix4(&skin4x4Mat, &bone->mOffsetMatrix);
        CpyMat(skin4x4Mat, skin3x3Mat); // Extract 3x3 matrix from 4x4 matrix.

        for (unsigned int i = 0; i < bone->mNumWeights; ++i) {
            unsigned int vId = bone->mWeights[i].mVertexId;
            float weight = bone->mWeights[i].mWeight;

            aiVector3D position = assimpMesh->mVertices[vId];
            aiTransformVecByMatrix4(&position, &skin4x4Mat);
            mesh.vertexData[vId].position.x += position.x * weight;
            mesh.vertexData[vId].position.y += position.y * weight;
            mesh.vertexData[vId].position.z += position.z * weight;

            aiVector3D normal = assimpMesh->mNormals[vId];
            aiTransformVecByMatrix3(&normal, &skin3x3Mat);
            mesh.vertexData[vId].normal.x += normal.x * weight;
            mesh.vertexData[vId].normal.y += normal.y * weight;
            mesh.vertexData[vId].normal.z += normal.z * weight;
        }
    }
}

void Model::AninmateMesh(const aiAnimation* animation, const float tick, Mesh& mesh) {
    const int frame = static_cast<int>(std::floor(tick));
    const float t = tick - frame;

    aiQuaternion r;
    for (unsigned int i = 0; i < animation->mNumChannels; ++i) {
        const aiNodeAnim* channel = animation->mChannels[i];
        aiNode* node = FindNode(mesh.rootNode, channel->mNodeName.data);
        aiQuatKey* r0 = channel->mRotationKeys + (frame + 0) % channel->mNumRotationKeys;
        aiQuatKey* r1 = channel->mRotationKeys + (frame + 1) % channel->mNumRotationKeys;
        Mix(r0->mValue, r1->mValue, t, r);
        ComposeMatrix(r, node->mTransformation);
    }

    TransfromMesh(mesh);
}

void Model::Mix(aiQuaternion& q1, const aiQuaternion& q2, float t, aiQuaternion& result) {
    aiQuaternion tmp;
    if (Dot(q1, q2) < 0) {
        tmp.x = -q1.x;
        tmp.y = -q1.y;
        tmp.z = -q1.z;
        tmp.w = -q1.w;
        q1 = tmp;
    }
    result.x = q1.x + t * (q2.x - q1.x);
    result.y = q1.y + t * (q2.y - q1.y);
    result.z = q1.z + t * (q2.z - q1.z);
    result.w = q1.w + t * (q2.w - q1.w);
    Normalize(result);
}

float Model::Dot(const aiQuaternion& q1, const aiQuaternion& q2) {
    return q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
}

void Model::Normalize(aiQuaternion& q) {
    float d = sqrt(Dot(q, q));
    if (d >= 0.00001) {
        d = 1 / d;
        q.x *= d;
        q.y *= d;
        q.z *= d;
        q.w *= d;
    }
    else {
        q.x = q.y = q.z = 0;
        q.w = 1;
    }
}

void Model::ComposeMatrix(const aiQuaternion& q, aiMatrix4x4& m) {
    // Rotation
    m.a1 = 1 - 2 * (q.y * q.y + q.z * q.z);
    m.a2 = 2 * (q.x * q.y - q.z * q.w);
    m.a3 = 2 * (q.x * q.z + q.y * q.w);
    m.b1 = 2 * (q.x * q.y + q.z * q.w);
    m.b2 = 1 - 2 * (q.x * q.x + q.z * q.z);
    m.b3 = 2 * (q.y * q.z - q.x * q.w);
    m.c1 = 2 * (q.x * q.z - q.y * q.w);
    m.c2 = 2 * (q.y * q.z + q.x * q.w);
    m.c3 = 1 - 2 * (q.x * q.x + q.y * q.y);
   
    // TODO Scale

    // TODO Translation

    m.d1 = 0; m.d2 = 0; m.d3 = 0; m.d4 = 1;
}

aiNode* Model::FindNode(aiNode* node, const char* name) {
    if (!std::strcmp(name, node->mName.data))
        return node;
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        aiNode* found = FindNode(node->mChildren[i], name);
        if (found)
            return found;
    }
    return nullptr;
}

void Model::TransformNode(const aiNode* node, aiMatrix4x4& transformMat) {
    if (node->mParent) {
        TransformNode(node->mParent, transformMat);
        aiMultiplyMatrix4(&transformMat, &node->mTransformation);
    }
    else {
        transformMat = node->mTransformation;
    }
}

void Model::CpyVec(const aiVector3D& aiVec, glm::vec3& glmVec) {
    glmVec.x = aiVec.x;
    glmVec.y = aiVec.y;
    glmVec.z = aiVec.z;
}

void Model::CpyVec(const aiVector3D& aiVec, glm::vec2& glmVec) {
    glmVec.x = aiVec.x;
    glmVec.y = aiVec.y;
}


void Model::CpyVec(const aiVector2D& aiVec, glm::vec2& glmVec) {
    glmVec.x = aiVec.x;
    glmVec.y = aiVec.y;
}

//void Model::CpyMat(const aiMatrix4x4& aiMat, glm::mat4& glmMat) {
//    glmMat[0][0] = aiMat.a1;
//    glmMat[0][1] = aiMat.a2;
//    glmMat[0][2] = aiMat.a3;
//    glmMat[0][3] = aiMat.a4;
//
//    glmMat[1][0] = aiMat.b1;
//    glmMat[1][1] = aiMat.b2;
//    glmMat[1][2] = aiMat.b3;
//    glmMat[1][3] = aiMat.b4;
//
//    glmMat[2][0] = aiMat.c1;
//    glmMat[2][1] = aiMat.c2;
//    glmMat[2][2] = aiMat.c3;
//    glmMat[2][3] = aiMat.c4;
//
//    glmMat[3][0] = aiMat.d1;
//    glmMat[3][1] = aiMat.d2;
//    glmMat[3][2] = aiMat.d3;
//    glmMat[3][3] = aiMat.d4;
//}

void Model::CpyMat(const aiMatrix4x4& aiMat4, aiMatrix3x3& aiMat3) {
    aiMat3.a1 = aiMat4.a1;
    aiMat3.a2 = aiMat4.a2;
    aiMat3.a3 = aiMat4.a3;

    aiMat3.b1 = aiMat4.b1;
    aiMat3.b2 = aiMat4.b2;
    aiMat3.b3 = aiMat4.b3;

    aiMat3.c1 = aiMat4.c1;
    aiMat3.c2 = aiMat4.c2;
    aiMat3.c3 = aiMat4.c3;
}
