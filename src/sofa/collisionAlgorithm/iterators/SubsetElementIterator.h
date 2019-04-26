﻿#pragma once

#include <sofa/collisionAlgorithm/BaseElementIterator.h>
#include <sofa/collisionAlgorithm/geometry/PointGeometry.h>

namespace sofa
{

namespace collisionAlgorithm
{

//Internal iterator of elements
class SubsetElementIterator : public BaseElementIterator {
public:
    SubsetElementIterator(BaseGeometry * geo, const std::set<unsigned> & subsetElements)
    : BaseElementIterator(geo)
    , m_container(geo)
    , m_subsetElements(subsetElements) {
        m_iterator = m_subsetElements.cbegin();
    }

    void next() {
        m_iterator++;
    }

    unsigned id() const {
        return *m_iterator;
    }

    bool end(unsigned ) const{
        return m_iterator==m_subsetElements.cend();
    }
    BaseProximity::SPtr project(const defaulttype::Vector3 & P) const override {
        return m_container->begin(id())->project(P);
    }

    BaseProximity::SPtr center() const override {
        return m_container->begin(id())->center();
    }

    defaulttype::BoundingBox getBBox() const override {
        return m_container->begin(id())->getBBox();
    }


    BaseGeometry * m_container;
    const std::set<unsigned> m_subsetElements;
    std::set<unsigned>::iterator m_iterator;
};



}

}
