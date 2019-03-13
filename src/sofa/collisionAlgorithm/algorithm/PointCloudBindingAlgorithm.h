#pragma once

#include <sofa/collisionAlgorithm/BaseAlgorithm.h>
#include <sofa/collisionAlgorithm/BaseGeometry.h>

namespace sofa {

namespace collisionAlgorithm {

class PointCloudBindingAlgorithm : public BaseAlgorithm
{
public:
    typedef sofa::defaulttype::Vector3 Vector3;
    SOFA_CLASS(PointCloudBindingAlgorithm, BaseAlgorithm);

    Data<double> d_maxDist;

    PointCloudBindingAlgorithm();

    void processAlgorithm(BaseElementIterator::UPtr it, const BaseGeometry * g2, helper::vector< PairDetection > & output);

    static void bind(const std::vector<defaulttype::Vector3> & p1, const std::vector<defaulttype::Vector3> & p2, helper::vector<int> & bindId, helper::vector<int> & invBind, double maxDist);
};

}

}
