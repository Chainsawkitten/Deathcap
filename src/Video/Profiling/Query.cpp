#include "Query.hpp"

#include <GL/glew.h>
#include <Utility/Log.hpp>

using namespace Video;

Query::Query(Type type) : active(false) {
    glGenQueries(2, queries);

    this->type = type;
    switch (type) {
        case TIME_ELAPSED:
            target = GL_TIMESTAMP;
            break;
        default:
            assert(false);
    }
}

Query::~Query() {
    glDeleteQueries(2, queries);
}

void Query::Begin() {
    if (active) {
        Log() << "Query::Begin warning: Already active.\n";
        return;
    }

    if (type == TIME_ELAPSED)
        glQueryCounter(queries[0], target);
    else {
        //glBegin();
    }

    active = true;
}

void Query::End() {
    if (!active) {
        Log() << "Query::End warning: Not active.\n";
        return;
    }

    glQueryCounter(queries[1], target);

    active = false;
}

Query::Type Query::GetType() const {
    return type;
}

std::uint64_t Query::Resolve() const {
    if (active) {
        Log() << "Query::Resolve warning: Can't resolve query while active.\n";
        return 0;
    }

    GLuint64 result[2];
    for (int i = 0; i < 2; ++i) {
        GLuint64 available = GL_FALSE;
        while (available == GL_FALSE)
            glGetQueryObjectui64v(queries[i], GL_QUERY_RESULT_AVAILABLE, &available);
        glGetQueryObjectui64v(queries[i], GL_QUERY_RESULT, &result[i]);
    }

    return result[1] - result[0];
}
