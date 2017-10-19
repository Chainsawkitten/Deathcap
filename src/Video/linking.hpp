#pragma once

#ifdef _WIN32
#ifdef Video_EXPORTS
#define VIDEO_API __declspec(dllexport)
#else
#define VIDEO_API __declspec(dllimport)
#endif
#else
#define VIDEO_API
#endif
