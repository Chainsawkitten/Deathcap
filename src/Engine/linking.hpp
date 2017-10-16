#pragma once

#ifdef _WIN32
    #ifdef Engine_EXPORTS
        #define ENGINE_EXPORT __declspec(dllexport)
    #else
        #define ENGINE_EXPORT __declspec(dllimport)
    #endif
#else
    #define ENGINE_EXPORT
#endif
