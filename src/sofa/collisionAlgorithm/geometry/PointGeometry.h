#pragma once

#include <sofa/collisionAlgorithm/BaseGeometry.h>
#include <sofa/collisionAlgorithm/iterators/DefaultElementIterator.h>
#include <sofa/collisionAlgorithm/proximity/PointProximity.h>
#include <sofa/collisionAlgorithm/elements/PointElement.h>
#include <sofa/collisionAlgorithm/toolbox/PointToolBox.h>

namespace sofa {

namespace collisionAlgorithm {

template<class DataTypes>
class PointGeometry : public TBaseGeometry<DataTypes,PointElement>, public PointElementGeometry {
public:
    typedef DataTypes TDataTypes;
    typedef PointElement ELEMENT;
    typedef PointGeometry<DataTypes> GEOMETRY;
    typedef TBaseGeometry<DataTypes,ELEMENT> Inherit;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef core::objectmodel::Data< VecCoord >        DataVecCoord;

    SOFA_CLASS(GEOMETRY,Inherit);

    Data<double> d_drawRadius;

    PointGeometry()
    : d_drawRadius(initData(&d_drawRadius, (double) 1.0, "drawRadius", "radius of drawing")) {}

    void init() {
        const helper::ReadAccessor<DataVecCoord> & pos = this->getState()->read(core::VecCoordId::position());

        for (unsigned i=0;i<pos.size();i++) {
            m_elements.push_back(this->createElement(this,i));
        }
    }

    inline BaseElement::Iterator begin(Index eid = 0) const override {
        return BaseElement::Iterator(new TDefaultElementIterator(m_elements,eid));
    }

//    void draw(const core::visual::VisualParams *vparams) override {
//        if(!(this->d_draw.getValue())) return;
////        this->drawNormals(vparams);
////        if (! vparams->displayFlags().getShowCollisionModels()) return;
//        if (! vparams->displayFlags().getShowCollisionModels()) {
//            return ;
//        }
//        const sofa::type::RGBAColor & color = this->d_color.getValue();
//        if (color[3] == 0.0) return;
//        if (d_drawRadius.getValue() == 0.0) return;

//        const helper::ReadAccessor<DataVecCoord> & pos = this->getState()->read(core::VecCoordId::position());

//        glColor4f(color[0],color[1],color[2],color[3]);

//        for(auto it=this->begin();it != this->end();it++) {
//            vparams->drawTool()->drawSphere(pos[it->id()],d_drawRadius.getValue());
//        }
//    }

    type::Vector3 getPosition(unsigned pid) const override {
        const helper::ReadAccessor<DataVecCoord> & pos = this->getState()->read(core::VecCoordId::position());
        return pos[pid];
    }

    BaseProximity::SPtr createProximity(unsigned p0) const override {

    }

private:
    std::vector<PointElement::SPtr> m_elements;
};

}

}
