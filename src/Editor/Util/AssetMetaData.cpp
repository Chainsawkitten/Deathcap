#include "AssetMetaData.hpp"
#include <fstream>
#include <Utility/Log.hpp>

bool AssetMetaData::SaveMetaData(const char* filepath, MeshImportData* metaData) {
    Json::Value node;
    ToJson(metaData, node);
    Save(filepath, node);
    return true;
}

bool AssetMetaData::LoadMetaData(const char* filepath, MeshImportData* metaData) {
    Json::Value node;
    Load(filepath, node);
    FromJson(node, metaData);
    return metaData;
}

void AssetMetaData::ToJson(const MeshImportData* metaData, Json::Value& node) {
    node["triangulate"] = metaData->triangulate;
    node["importNormals"] = metaData->importNormals;
    node["importTangents"] = metaData->importTangents;
    node["CPU"] = metaData->CPU;
    node["GPU"] = metaData->GPU;
}

void AssetMetaData::FromJson(const Json::Value& node, MeshImportData* metaData) {
    metaData->triangulate = node.get("triangulate", true).asBool();
    metaData->importNormals = node.get("importNormals", true).asBool();
    metaData->importTangents = node.get("importTangents", false).asBool();
    metaData->CPU = node.get("CPU", false).asBool();
    metaData->GPU = node.get("GPU", true).asBool();
}

bool AssetMetaData::Save(const char* filepath, Json::Value& node) {
    // Save to file.
    std::ofstream file(filepath);

    // Return if file is not open.
    if (!file.is_open()) {
        Log() << "AssetMetaData::Save; Couldn't open file: " << filepath << "\n";
        return false;
    }

    file << node;
    file.close();

    return true;
}

bool AssetMetaData::Load(const char* filepath, Json::Value& node) {
    // Load Json from file.
    std::ifstream file(filepath);

    // Return if file is not open.
    if (!file.is_open()) {
        Log() << "AssetMetaData::Load; Couldn't open file: " << filepath << "\n";
        return false;
    }

    file >> node;
    file.close();

    return true;
}