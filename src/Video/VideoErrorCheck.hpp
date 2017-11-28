#pragma once

#include "linking.hpp"
#include <string>

/// Run video error check.
class VideoErrorCheck {
    public:
        /// Start video error check.
        /**
         * @param name Name the scope to check for errors.
         */
        VIDEO_API explicit VideoErrorCheck(const std::string& name);

        /// End video error check.
        VIDEO_API ~VideoErrorCheck();

    private:
        std::string name;
};

#define VIDEO_ERROR_CHECK(name) VideoErrorCheck __videoErrorCheckInstance(name)
