#include "MemTrack.hpp"
#define MEMTRACK_NEW MemTrack::MemStamp(__FILE__, __LINE__) * new
#define new MEMTRACK_NEW
