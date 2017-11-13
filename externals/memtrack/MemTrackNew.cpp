#include "MemTrack.hpp"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <typeinfo>
#include <new>

void *operator new(size_t size)
{
    void *p = MemTrack::TrackMalloc(size);
    if (p == NULL) throw std::bad_alloc();
    return p;
}

/* ---------------------------------------- operator delete */

void operator delete(void *p)
{
    MemTrack::TrackFree(p);
}

/* ---------------------------------------- operator new[] */

void *operator new[](size_t size)
{
    void *p = MemTrack::TrackMalloc(size);
    if (p == NULL) throw std::bad_alloc();
    return p;
}

/* ---------------------------------------- operator delete[] */

void operator delete[](void *p)
{
    MemTrack::TrackFree(p);
}