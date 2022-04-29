#include <sofa/collisionAlgorithm/algorithm/ElementsIntersectionAlgorithm.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/collisionAlgorithm/BaseElement.h>
#include <sofa/collisionAlgorithm/BaseGeometry.h>
#include <sofa/collisionAlgorithm/elements/TetrahedronElement.h>
#include <sofa/collisionAlgorithm/geometry/TetrahedronGeometry.h>

namespace sofa::collisionAlgorithm {

SOFA_DECL_CLASS(FindClosestProximityAlgorithm)

int ElementsIntersectionAlgorithmClass = core::RegisterObject("ElementsIntersectionAlgorithm")
.add< ElementsIntersectionAlgorithm >();



static void doIntersection (BaseElement* elem, ElementIterator::SPtr itdest, std::vector<BaseElement::SPtr> & IntersectRes) {

    for (; ! itdest->end();itdest++) {
        auto edest = itdest->element();
        if (edest == nullptr) continue;

//        std::cout << "doIntersection" << std::endl;
        Operations::IntersectOperation::FUNC intersectionOp = Operations::IntersectOperation::get(elem->getOperationsHash() ,itdest->getOperationsHash());
//        std::cout << "between get and intersectionOp" << std::endl;
        auto elemIntersection = intersectionOp(elem,edest);
        std::cout << "after intersectionOp" << std::endl;
        if (elemIntersection == NULL) continue;

        IntersectRes.push_back(elemIntersection); // Be careful with the case of an edge being parallel to a triangle: possibility of intersection on two edges
                                                  // Those edges may belong to different triangles, only one shoud be considered
                                                  // OR : before filling outputDetection, delete intersection elements being at the same location for example
                                                  // IMPORTANT CASE TO HANDLE !!
    }

}


//By default the elements of the geometry are considered
static void genericIntersection(BaseElement* elem, BaseGeometry * geo, std::vector<BaseElement::SPtr> & IntersectRes) {std::cout << "genericIntersection" << std::endl;
    doIntersection(elem,geo->begin(),IntersectRes);
}

//By default the elements of the geometry are considered
IntersectWithGeometryOperation::GenericOperation::FUNC IntersectWithGeometryOperation::getDefault() const {
    return &genericIntersection;
}


//In case of a tetrahedral geometry, the triangles are considered instead of the tetrahedra
template<class TETRAGEOM>
static void IntersectionWithTetraGeometry (BaseElement* elem, BaseGeometry * geo, std::vector<BaseElement::SPtr> & IntersectRes) { std::cout << "IntersectionWithTetraGeometry" << std::endl;
    TETRAGEOM * tetraGeo = (TETRAGEOM*) geo;
    doIntersection(elem, tetraGeo->triangleBegin(), IntersectRes);
}



template <typename DataTypes>
int register_IntersectionOperationTetraGeometry = IntersectWithGeometryOperation::register_func<TetrahedronElement>(&IntersectionWithTetraGeometry<TetrahedronGeometry<DataTypes>>);

} // namespace sofa::collisionAlgorithm


