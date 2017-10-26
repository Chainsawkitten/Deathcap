#pragma once

#include <assimp/quaternion.h>
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <vector>
#include "../linking.hpp"

struct aiAnimation;

namespace Geometry {
    /// An animation loaded from a file.
    class Animation {
            public:
        /// Animation channel representing rotation, position and scale transformations. Describes the animation of a single node.
        struct AnimChannel {
            /// Rotation key.
            struct QuatKey {
                /// Time of this key.
                double time;

                /// Value of this key.
                aiQuaternion value;
            };

            /// Scale and position keys.
            struct VectorKey {
                /// Time of this key.
                double time;

                /// Value of this key.
                glm::vec3 value;
            };

            /// Find rotation key index.
            /**
                 * @param animationTime Duration of the animation in seconds.
                 * @return Index of key.
                 */
            ENGINE_API std::size_t FindRotKey(float animationTime) const;

            /// Find position key index.
            /**
                 * @param animationTime Duration of the animation in seconds.
                 * @return Index of key.
                 */
            ENGINE_API std::size_t FindPosKey(float animationTime) const;

            /// Find scale key index.
            /**
                 * @param animationTime Duration of the animation in seconds.
                 * @return Index of key.
                 */
            ENGINE_API std::size_t FindSclKey(float animationTime) const;

            /// Vector containing rotation keys.
            std::vector<QuatKey> rotKeys;

            /// Vector containing position keys.
            std::vector<VectorKey> posKeys;

            /// Vector containing scale keys.
            std::vector<VectorKey> sclKeys;

            /// The name of the node affected by this animation.
            std::string trgNodeName = "";
        };

        /// Create new empty animation.
        /**
             * The created animation has to be loaded later using Load.
             */
        ENGINE_API Animation();

        /// Create new animation.
        /**
             * @param aAnimation Pointer to assimp animation.
             */
        ENGINE_API Animation(const aiAnimation* aAnimation);

        /// Destructor.
        ENGINE_API ~Animation();

        /// Load animation.
        /**
             * @param aAnimation Pointer to assimp animation.
             */
        ENGINE_API void Load(const aiAnimation* aAnimation);

        /// Load animation.
        /**
             * @param name Channel name to find.
             * @return Pointer to channel. Returns nullptr if channel isn't found.
             */
        ENGINE_API const AnimChannel* FindChannel(const std::string& name) const;

        /// Interpolate animation channel.
        /**
             * @param scaling Target scaling vector.
             * @param animationTime Time fo animation.
             * @param channel Animation channel.
             */
        ENGINE_API static void CalcInterpolatedScaling(glm::vec3& scaling, float animationTime, const AnimChannel* channel);

        /// Interpolate animation channel.
        /**
             * @param rotation Target rotation quaternion.
             * @param animationTime Time fo animation.
             * @param channel Animation channel.
             */
        ENGINE_API static void CalcInterpolatedRotation(aiQuaternion& rotation, float animationTime, const Animation::AnimChannel* channel);

        /// Interpolate animation channel.
        /**
             * @param translation Target translation vector.
             * @param animationTime Time fo animation.
             * @param channel Animation channel.
             */
        ENGINE_API static void CalcInterpolatedPosition(glm::vec3& translation, float animationTime, const Animation::AnimChannel* channel);

        /// Animation name.
        std::string name;

        /// Duration of the animation in ticks.
        double duration;

        /// Animation ticks per second.
        double ticksPerSecond;

            private:
        std::map<std::string, std::size_t> channelIndexMap;
        std::vector<AnimChannel> channels;
    };
}
