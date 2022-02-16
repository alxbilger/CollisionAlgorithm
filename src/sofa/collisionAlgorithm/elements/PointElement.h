﻿#pragma once

#include <sofa/collisionAlgorithm/BaseElement.h>

namespace sofa::collisionAlgorithm {

class PointElement : public TBaseElement<std::function<BaseProximity::SPtr(const PointElement *)> > {
public:

    using Inherit = TBaseElement;
    typedef std::shared_ptr<PointElement> SPtr;

    PointElement(unsigned eid, unsigned p,Inherit::ProxCreatorFunc f)
    : TBaseElement(eid, f)
    , m_point(p) {}

    inline BaseProximity::SPtr createProximity() const {
        return m_createProxFunc(this);
    }

    inline unsigned getP0() const { return m_point; }

    void getControlProximities(std::vector<BaseProximity::SPtr> & res) const override {
        res.push_back(createProximity());
    }

    void draw(const core::visual::VisualParams * /*vparams*/) override {
        type::Vector3 p0 = createProximity()->getPosition();

        glBegin(GL_POINT);
            glVertex3dv(p0.data());
        glEnd();
    }

private:
    unsigned m_point;
};


}
