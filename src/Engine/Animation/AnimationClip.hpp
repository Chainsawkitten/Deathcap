#include <string>
#include <vector>

namespace Animation {
    /// An animation loaded from a file.
    class AnimationClip {
        public:
            void Load(const std::string& name);

            std::string name;
        private:

    };
}