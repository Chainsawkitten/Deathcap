#include "Query.hpp"
#include <assert.h>

using namespace Video;

Query::Query(GLenum target) {
    assert(target == GL_TIMESTAMP);


}

Query::~Query() {

}
