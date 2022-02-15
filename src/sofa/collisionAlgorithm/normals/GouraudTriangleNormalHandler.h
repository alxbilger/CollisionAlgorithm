#pragma once

#include <sofa/collisionAlgorithm/CollisionPipeline.h>
#include <sofa/collisionAlgorithm/geometry/TriangleGeometry.h>

namespace sofa::collisionAlgorithm {

template<class DataTypes>
class GouraudTriangleNormalHandler : public CollisionComponent {
public:

    SOFA_CLASS(SOFA_TEMPLATE(GouraudTriangleNormalHandler,DataTypes), CollisionComponent);

    typedef TriangleGeometry<DataTypes> GEOMETRY;
    typedef typename GEOMETRY::ELEMENT ELEMENT;

    core::objectmodel::SingleLink<GouraudTriangleNormalHandler<DataTypes>,GEOMETRY,BaseLink::FLAG_STRONGLINK|BaseLink::FLAG_STOREPATH> l_geometry;

    GouraudTriangleNormalHandler()
    : l_geometry(initLink("geometry","Link to TriangleGeometry")){}

    class GouraudTriangleProximity : public DefaultTriangleProximity<DataTypes> {
    public:
        typedef sofa::core::behavior::MechanicalState<DataTypes> State;

        GouraudTriangleProximity(State * state, unsigned p0,unsigned p1, unsigned p2, double f0,double f1,double f2,const type::Vector3 & N)
        : DefaultTriangleProximity<DataTypes>(state,p0,p1,p2,f0,f1,f2)
        , m_N(N) {}

        virtual sofa::type::Vector3 getNormal() const override { return m_N; }

    private:
        const type::Vector3 & m_N;
    };

    void init() {
        for (auto it = l_geometry->begin();it != l_geometry->end(); it++) {
            BaseElement::SPtr elmt = it->element();
            auto elmt_cast = elmt->cast<ELEMENT>();

            //change the behavior of elements
            elmt_cast->setProximityCreator(
                [=](const TriangleElement * elmt, double f0,double f1,double f2) -> BaseProximity::SPtr {
                    return BaseProximity::SPtr(new GouraudTriangleProximity(l_geometry->getState(),
                                                                          elmt->getP0(),elmt->getP1(),elmt->getP2(),
                                                                          f0,f1,f2,
                                                                          elmt->getTriangleInfo().N));
                }
            );
        }
    }

    void prepareDetection() override {}

};

}
