#include "SteamAudioInterface.hpp"


SteamAudioInterface::SteamAudioInterface() {
    playerDir = new IPLVector3{ 0, 0, 0 };
    playerPos = new IPLVector3{ 0, 0, 0 };
    playerUp = new IPLVector3{ 0, 0, 0 };

    simSettings.sceneType = IPL_SCENETYPE_PHONON;
    simSettings.numRays = 64000;
    simSettings.numDiffuseSamples = 32;
    simSettings.numBounces = 2;
    simSettings.irDuration = 1.0;
    simSettings.ambisonicsOrder = 0;
    simSettings.maxConvolutionSources = 0;
}

SteamAudioInterface::~SteamAudioInterface() {
    // Might have to remove these depending on how the pointers are sent to this class
    delete playerDir;
    delete playerPos;
    delete playerUp;
}

void SteamAudioInterface::SetContext(IPLLogFunction logCallback, IPLAllocateFunction allocCallback, IPLFreeFunction freeCallback) {

}

void SteamAudioInterface::CreateScene(uint32_t numMaterials) {
    iplCreateScene(context, NULL, simSettings, numMaterials, scene);
}

void SteamAudioInterface::FinalizeScene(IPLFinalizeSceneProgressCallback progressCallback) {
    iplFinalizeScene(scene, NULL);
}

SteamAudioInterface::SaveData SteamAudioInterface::SaveFinalizedScene() {
    SaveData saveData;

    // Call once first only to figure out the size of the saved scene.
    saveData.sceneSize = iplSaveFinalizedScene(scene, NULL);

    // Actually save the scene.
    saveData.scene = new IPLbyte[saveData.sceneSize];
    iplSaveFinalizedScene(scene, saveData.scene);

    return saveData;
}

void SteamAudioInterface::LoadFinalizedScene(SaveData data) {
    iplLoadFinalizedScene(context, simSettings, data.scene, data.sceneSize, NULL, NULL, scene);
}

void SteamAudioInterface::SetSceneMaterial(uint32_t matIndex, IPLMaterial material) {
    iplSetSceneMaterial(scene, matIndex, material);
}

IPLhandle * SteamAudioInterface::CreateStaticMesh(std::vector<IPLVector3> vertices, std::vector<IPLTriangle> indices, int materialIndex) {
    IPLhandle* mesh;

    // Create mesh
    iplCreateStaticMesh(scene, vertices.size(), indices.size() / 3, mesh);
    iplSetStaticMeshVertices(scene, mesh, vertices.data());
    iplSetStaticMeshTriangles(scene, mesh, indices.data());
    iplSetStaticMeshMaterials(scene, mesh, &materialIndex);

    return nullptr;
}

void SteamAudioInterface::CreateEnvironment(IPLSimulationSettings settings) {
    iplCreateEnvironment(context, NULL, simSettings, scene, NULL, environment);
}

void SteamAudioInterface::SetPlayer(IPLVector3 * playerPos, IPLVector3 * playerDir, IPLVector3 * playerUp) {

}

void SteamAudioInterface::Process(float* input, uint32_t samples,  IPLVector3 * sourcePosition, float sourceRadius) {

    //Create a steam audio buffer
    IPLAudioFormat format;
    format.channelLayoutType = IPL_CHANNELLAYOUTTYPE_SPEAKERS;
    format.channelLayout = IPL_CHANNELLAYOUT_MONO; //Only take mono input
    format.numSpeakers = 1;
    format.speakerDirections = NULL;
    format.channelOrder = IPL_CHANNELORDER_INTERLEAVED;

    IPLAudioBuffer buf;
    buf.format = format;
    buf.numSamples = samples;
    buf.interleavedBuffer = input;
    buf.deinterleavedBuffer = NULL;

    sAudio.Process(buf, 0, 0, 0, 0, 0);
}

float * SteamAudioInterface::GetProcessed(uint32_t* numSamples) {
    IPLAudioBuffer* finalBuf = new IPLAudioBuffer;
    sAudio.GetFinalMix(finalBuf, numSamples);

    return finalBuf->interleavedBuffer;
}
