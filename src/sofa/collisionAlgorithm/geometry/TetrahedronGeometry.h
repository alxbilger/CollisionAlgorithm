#pragma once

#include <sofa/collisionAlgorithm/geometry/TriangleGeometry.h>
#include <sofa/collisionAlgorithm/elements/TetrahedronElement.h>
#include <sofa/collisionAlgorithm/proximity/TetrahedronProximity.h>
#include <sofa/core/topology/BaseMeshTopology.h>

namespace sofa::collisionAlgorithm {

template<class DataTypes>
class TetrahedronGeometry : public TriangleGeometry<DataTypes> {
public:
    typedef DataTypes TDataTypes;
    typedef TetrahedronElement ELEMENT;
    typedef TetrahedronGeometry<DataTypes> GEOMETRY;
    typedef TBaseGeometry<DataTypes> Inherit;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef core::objectmodel::Data< VecCoord >        DataVecCoord;
    typedef std::function<BaseProximity::SPtr(const TetrahedronElement * elmt,double f0,double f1,double f2,double f3)> ProximityCreatorFunc;

    void buildTetrahedronElements() override {
        for (unsigned i=0;i<this->l_topology->getNbTetrahedra();i++) {
            auto triId = this->l_topology->getTrianglesInTetrahedron(i);

            TriangleElement::SPtr triangle0 = this->triangleElements()[triId[0]];
            TriangleElement::SPtr triangle1 = this->triangleElements()[triId[1]];
            TriangleElement::SPtr triangle2 = this->triangleElements()[triId[2]];
            TriangleElement::SPtr triangle3 = this->triangleElements()[triId[3]];

            this->tetrahedronElements().insert(TetrahedronElement::create(triangle0, triangle1, triangle2, triangle3));
        }
    }

    ElementIterator::SPtr begin(unsigned id = 0) const override { return this->tetrahedronBegin(id); }

};

}
