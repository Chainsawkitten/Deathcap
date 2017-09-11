#include "Model.hpp"

#include <glm/gtc/quaternion.hpp>
#include "../Hymn.hpp"
#include "../Util/FileSystem.hpp"

using namespace Geometry;

Model::Model() {
    
}

Model::~Model() {
    
}

Json::Value Model::Save() const {
    Json::Value model;
    model["name"] = name;
    model["extension"] = extension;
    model["type"] = "Static"; // GetType() == STATIC ? "Static" : "Skin";
    return model;
}

void Model::Load(const Json::Value& node) {
    name = node.get("name", "").asString();
    extension = node.get("extension", "").asString();
    Load((Hymn().GetPath() + FileSystem::DELIMITER + "Models" + FileSystem::DELIMITER + name + "." + extension).c_str());
}

void Model::Load(const char* filename) {
    if (assetFile.Open(filename, AssetFileHandler::READ)) {
        assetFile.LoadMeshData(0);
        AssetFileHandler::MeshData * meshData = assetFile.GetStaticMeshData();
        GenerateVertexBuffer(vertexBuffer, meshData->vertices, meshData->numVertices);
        GenerateIndexBuffer(meshData->indices, meshData->numIndices, indexBuffer);
        GenerateVertexArray(vertexBuffer, indexBuffer, vertexArray);
        CreateAxisAlignedBoundingBox(meshData->aabbDim, meshData->aabbOrigin, meshData->aabbMinpos, meshData->aabbMaxpos);
        assetFile.Close();
    }
}

Model::Type Model::GetType() {
    return STATIC;
}

void Model::GenerateVertexBuffer(GLuint& vertexBuffer,
    Video::Geometry::VertexType::StaticVertex * vertices, unsigned int numVerticies) {
    vertexBuffer = Video::Geometry::VertexType::StaticVertex::GenerateVertexBuffer(
        vertices, numVerticies);
}

void Model::GenerateVertexArray(const GLuint vertexBuffer, const GLuint indexBuffer, GLuint& vertexArray) {
    vertexArray = Video::Geometry::VertexType::StaticVertex::GenerateVertexArray(vertexBuffer, indexBuffer);
}