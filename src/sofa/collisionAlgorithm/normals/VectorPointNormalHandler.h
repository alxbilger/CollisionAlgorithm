#pragma once

#include <sofa/collisionAlgorithm/CollisionPipeline.h>
#include <sofa/collisionAlgorithm/geometry/PointGeometry.h>

namespace sofa::collisionAlgorithm {


//#ifdef NOT_COMPILING

template<class DataTypes>
class VectorPointNormalHandler : public CollisionComponent {
public:

    SOFA_CLASS(SOFA_TEMPLATE(VectorPointNormalHandler,DataTypes), CollisionComponent);

    Data<type::vector<type::Vector3>> d_normals;

    typedef PointGeometry<DataTypes> GEOMETRY;
    typedef typename GEOMETRY::ELEMENT ELEMENT;

    core::objectmodel::SingleLink<VectorPointNormalHandler<DataTypes>,PointGeometry<DataTypes>,BaseLink::FLAG_STRONGLINK|BaseLink::FLAG_STOREPATH> l_geometry;

    VectorPointNormalHandler()
    : d_normals(initData(&d_normals, "normals", "Vector of normals"))
    , l_geometry(initLink("geometry","Link to Geometry")){}

//    class VectorPointNormalProximity : public PointProximity {
//    public:
//        typedef sofa::core::behavior::MechanicalState<DataTypes> State;
//        typedef typename DataTypes::VecCoord VecCoord;
//        typedef core::objectmodel::Data< VecCoord >        DataVecCoord;

//        VectorPointNormalProximity(State * s, BaseProximity::SPtr p0, const type::vector<type::Vector3> & normals)
//        : PointProximity(p0)
//        , m_normals(normals){}

//        sofa::type::Vector3 getNormal() const override {
//            return m_normals[this->m_pid];
//        }

//    private:
//        const type::vector<type::Vector3> & m_normals;

//    };

    void init() {
//        l_geometry->setCreateProximity(
//                [=](const PointElement * elmt) -> BaseProximity::SPtr {
//                    return BaseProximity::create<VectorPointNormalProximity>(l_geometry->getState(),
//                                                                              elmt->getP0(),
//                                                                              d_normals.getValue());
//                }
//            );
        for (unsigned i=0; i<d_normals.getValue().size(); i++)
        {
            l_geometry->getTopoProxIdx(i)->setNormal(d_normals.getValue()[i]);
        }

    }

    void prepareDetection() override {}

};


//#endif

}
