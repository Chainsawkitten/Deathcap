#include "Log.hpp"

#include <ostream>
#include <iostream>

using namespace std;

ostream* Log::streams[NUMBER_OF_CHANNELS];

Log::Log(const Channel channel) {
    currentChannel = channel;
}

Log::~Log() {
    //fflush(stderr);
}

Log& Log::operator<<(const string text) {
    *streams[currentChannel] << text;
    return *this;
}

Log& Log::operator <<(const int value) {
    *streams[currentChannel] << value;
    return *this;
}

Log& Log::operator <<(const unsigned int value) {
    *streams[currentChannel] << value;
    return *this;
}

Log& Log::operator <<(const float value) {
    *streams[currentChannel] << value;
    return *this;
}

Log& Log::operator <<(const double value) {
    *streams[currentChannel] << value;
    return *this;
}

Log& Log::operator <<(const time_t value) {
    struct tm * timeinfo = localtime(&value);

    const unsigned int bufferLength = 24;

    char buffer[bufferLength] = {'\0'};
    strftime(buffer, bufferLength, "%Y-%m-%d %H:%M:%S", timeinfo);
    string const outString = string(buffer, bufferLength);
    
    *streams[currentChannel] << outString;

    return *this;
}

Log& Log::operator <<(const glm::vec2& value) {
    *streams[currentChannel] << "(" << value.x << "," << value.y << ")";
    return *this;
}

Log& Log::operator <<(const glm::vec3& value) {
    *streams[currentChannel] << "(" << value.x << "," << value.y << "," << value.z << ")";
    return *this;
}

Log & Log::operator<<(const glm::vec4 & value) {
    *streams[currentChannel] << "(" << value.x << "," << value.y << "," << value.z << "," << value.w << ")";
    return *this;
}

bool Log::SetupStream(const Channel channel, std::ostream* stream) {
    // Check if channel is outside the range of available channels
    if(channel < DEFAULT || channel >= NUMBER_OF_CHANNELS) {
        std::cout << "Error channel: out of range.";
        return false;
    }

    // Set the channel.
    streams[static_cast<int>(channel)] = stream;
    return true;
}

