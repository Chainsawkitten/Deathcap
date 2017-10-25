#include "Log.hpp"

#include <cstdio>

using namespace std;

Log::~Log() {
    fflush(stderr);
}

Log& Log::operator<<(string text) {
    fputs(text.c_str(), stderr);
    return *this;
}

Log& Log::operator <<(int value) {
    fprintf(stderr, "%i", value);
    return *this;
}

Log& Log::operator <<(unsigned int value) {
    fprintf(stderr, "%u", value);
    return *this;
}

Log& Log::operator <<(float value) {
    fprintf(stderr, "%f", value);
    return *this;
}

Log& Log::operator <<(double value) {
    fprintf(stderr, "%f", value);
    return *this;
}

Log& Log::operator <<(time_t value) {
    struct tm * timeinfo = localtime(&value);
    
    char buffer[24];
    strftime(buffer, 24, "%Y-%m-%d %H:%M:%S", timeinfo);
    *this << buffer;
    
    return *this;
}

Log& Log::operator <<(const glm::vec2& value) {
    fprintf(stderr, "(%f, %f)", value.x, value.y);
    return *this;
}

Log& Log::operator <<(const glm::vec3& value) {
    fprintf(stderr, "(%f, %f, %f)", value.x, value.y, value.z);
    return *this;
}

Log & Log::operator<<(const glm::vec4 & value) {
    fprintf(stderr, "(%f, %f, %f, %f)", value.x, value.y, value.z, value.w);
    return *this;
}

bool Log::SetupStream(const Channel channel, std::ostream* stream) {
    // Check if channel is outside the range of available channels
    if(channel < Channel::DEFAULT || channel >= Channel::NUMBER_OF_CHANNELS) {
        std::cout << "Error channel: out of range.";
        return false;
    }

    // Set the channel.
    streams[static_cast<int>(channel)] = stream;
    return true;
}

