#pragma once

#include <phonon.h>
#include <vector>
#include <cstdint>
#include "SteamAudio.hpp"
#include "../linking.hpp"

/// Interface to set up and send data to/from Steam Audio from the rest of the engine.
class SteamAudioInterface {
    public:

        /// Create default.
        SteamAudioInterface();

        /// Destructor.
        ~SteamAudioInterface();

        /// Sets the main context used by Steam Audio.
        /**
         * @param logCallback Callback for logging messages. Can be NULL. Default uses stdout.
         * @param allocCallback Callback for allocating memory. Can be NULL. Default uses malloc.
         * @param freeCallback Callback for freeing memory. Can be NULL. Default uses free.
         */
        void SetContext(IPLLogFunction logCallback, IPLAllocateFunction allocCallback, IPLFreeFunction freeCallback);
    
        /// Creates the scene object, to be populated with meshes.
        /**
         * @param settings The settings to use for simulation.
         * @param numMaterials Number of different materials used. May not be added or removed after Scene is created.
         */
        void CreateScene(uint32_t numMaterials);

        /// Finalizes the scene object.
        /**
         * @param progressCallback Callback to check the progress of the finalization. Can be NULL.
         */
        void FinalizeScene(IPLFinalizeSceneProgressCallback progressCallback); //Can also create Environment (until we start using multiple scenes)
    
        /// Everything needed to load a previously finalized scene stored in a byte array.
        struct SaveData {
            IPLSimulationSettings settings;
            int sceneSize;
            IPLbyte* scene;
        };

        /// Saves the finalized scene to a struct.
        /**
         * @return A struct containing everything needed to reconstruct the scene with LoadFinalizedScene().
         */
        SaveData SaveFinalizedScene();

        /// Loads a finalized scene from a struct.
        /**
         * @param data Struct containing all data relevant to reconstructing a scene
         */
        void LoadFinalizedScene(SaveData data);

        /// Specifies a single material used by the scene
        /**
         * @param matIndex Index of the material to set. Between 0 and N-1 where N is the value of numMaterials passed to CreateScene().
         * @param material The material properties to set.
         **/
        void SetSceneMaterial(uint32_t matIndex, IPLMaterial material);

        /// Creates a static mesh and adds it to the non-finalized scene.
        /**
         * @param verices Vector of all vertices of the mesh.
         * @param indices Vector of all indices of the mesh.
         * @param materialIndex Index of the material to be used. Previously specified in SetSceneMaterial.
         * @return A handle to the static mesh (automatically added to the scene).
         */
        IPLhandle* CreateStaticMesh(std::vector<IPLVector3> vertices, std::vector<IPLTriangle> indices, int materialIndex);

        /// Creates an Environment object for use by the audio engine internally.
        void CreateEnvironment();

        /// Give pointers to player positional data.
        /**
         * @param playerPos Player position.
         * @param playerDir Player forward.
         * @param playerUp Player up.
         **/
        void SetPlayer(IPLVector3 playerPos, IPLVector3 playerDir, IPLVector3 playerUp);

        /// Processes audio samples
        /**
         * @param input The audio buffer to be processed.
         * @param sourcePosition The position of the sound source.
         * @param sourceRadius The radius of the source. To determine how much of the source is occluded rather than have it be on/off.
         **/
        void Process(float* input, uint32_t samples, IPLVector3 sourcePosition, float sourceRadius); // Needs to be called in a way so that there's always at least one processed audio frame ready to go.

        /// Fetches the processed mix using all buffers sent through Process()
        /**
         * @param numSamples The number of samples in the final buffer.
         * @return A pointer to the processed buffer.
         **/
        float* GetProcessed(uint32_t* numSamples);

    private:

        IPLContext context;

        IPLSimulationSettings simSettings;

        SteamAudio sAudio;

        IPLhandle* scene;
        IPLhandle* environment;
};
