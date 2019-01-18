﻿#pragma once

#include <sofa/collisionAlgorithm/BaseElementIterator.h>
#include <sofa/collisionAlgorithm/BaseGeometry.h>
#include <sofa/collisionAlgorithm/proximity/PointProximity.h>

namespace sofa
{

namespace collisionAlgorithm
{

template<class GEOMETRY, class PROXIMITY>
class DefaultElementIterator : public BaseElementIterator {
    friend class Iterator;

public:
    DefaultElementIterator(const GEOMETRY * geo, unsigned start, unsigned end) {
        m_id = start;
        m_end = end;
        m_geometry = geo;
    }

    inline BaseProximity::SPtr project(const defaulttype::Vector3 & P) const {
        return PROXIMITY::project(m_geometry, m_id, P);
    }

    inline BaseProximity::SPtr center() const {
        return PROXIMITY::center(m_geometry, m_id);
    }

    inline defaulttype::BoundingBox getBBox() const {
        return PROXIMITY::getBBox(m_geometry, m_id);
    }

    virtual void next() {
        this->m_id++;
    }

    virtual bool end(const BaseGeometry * /*geo*/) const {
        return m_id>=m_end;
    }

    virtual unsigned id() const {
        return m_id;
    }

    template<class CONTAINER>
    static BaseElementIterator::UPtr create(const GEOMETRY * geo, const CONTAINER & container, unsigned start = 0) {
        return BaseElementIterator::UPtr(new DefaultElementIterator(geo,start,container.size()));
    }

private:
    unsigned m_id;
    unsigned m_end;
    const GEOMETRY * m_geometry;
};

}

}
