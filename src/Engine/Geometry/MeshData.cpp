#include "MeshData.hpp"

#ifdef USINGMEMTRACK
#include <MemTrackInclude.hpp>
#endif

using namespace Geometry;

Geometry::MeshData::~MeshData() {
    if (staticVertices)
        delete[] staticVertices;

    if (skinnedVertices)
        delete[] skinnedVertices;

    if (indices)
        delete[] indices;
}
