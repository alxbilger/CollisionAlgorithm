#include <Collision.h>
#include <qopengl.h>

namespace collisionAlgorithm {

Collision::Collision()
: p_type("out",IN,this) {
    m_dirty = true;
}

}

CONNECTABLE(collisionAlgorithm::Collision)