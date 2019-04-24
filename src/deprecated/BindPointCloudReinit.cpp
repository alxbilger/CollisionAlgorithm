#include "CollisionAlgorithm.h"
#include <sofa/core/behavior/MechanicalState.h>
#include <math.h>

namespace sofa {

namespace collisionAlgorithm {

void CollisionAlgorithm::BindPointCloudReinit(
    const helper::vector<defaulttype::Vector3> & p1,
    const helper::vector<defaulttype::Vector3> & p2 ,
    helper::vector<int> & bindId,

    double minDist
) {
    helper::vector<int> reinit(p1.size(),-1);
    bindId.swap(reinit);

    if (p1.empty()) return;
    if (p2.empty()) return;

    bool change = true;

    helper::vector<int> invBind;
    invBind.resize(p2.size(),-1);

    helper::vector<double> totaldistp1;
    totaldistp1.resize(p1.size(),0);
    helper::vector<double> totaldistp2;
    totaldistp2.resize(p2.size(),0);

    for (unsigned p=0;p<p1.size();p++) {
        for (unsigned q=0;q<p1.size();q++) {
            defaulttype::Vector3 P=p1[p];
            defaulttype::Vector3 Q=p1[q];
            totaldistp1[p]+=(P-Q).norm();
        }
        totaldistp1[p]/=(double)p1.size();
    }

    for (unsigned p=0;p<p2.size();p++) {
        for (unsigned q=0;q<p2.size();q++) {
            defaulttype::Vector3 P=p2[p];
            defaulttype::Vector3 Q=p2[q];
            totaldistp2[p]+=(P-Q).norm();
        }
        totaldistp2[p]/=(double)p2.size();
    }
    while (change) {
        change = false;

        for (unsigned p=0;p<p1.size();p++) {
            if (bindId[p] != -1) continue;

            int closestId = -1;
            double closestDist = std::numeric_limits<double>::max();

            //Find minimal distance
            for (unsigned i=0;i<p2.size();i++) {
  
                double dist = (totaldistp1[p]-totaldistp2[i])*(totaldistp1[p]-totaldistp2[i]);

                if (dist < closestDist && invBind[i] == -1) {
                    closestId = i;
                    closestDist = dist;
                }
            }
           if ((minDist!=0) && (closestDist > minDist)) closestId = -1;

            bindId[p] = closestId;
        }

        for (unsigned i=0;i<p1.size();i++) {
            if (bindId[i] == -1) continue;

            int & B = bindId[i]; //current point in p1
            int & C = invBind[bindId[i]]; // previous binded point in p2

            if (C == -1) { // the point has not yet been associated
                C = i;
            } else if (C != (int) i) { // the point is already associated
                int & A = bindId[C]; // previous binded point in p1

                change = true; // we retry the binding because two points are associated with the same point

                double d1 = (totaldistp1[A] - totaldistp2[C])*(totaldistp1[A] - totaldistp2[C]);
                double d2 = (totaldistp1[B] - totaldistp2[C])*(totaldistp1[B] - totaldistp2[C]);
                //double d1 = (p1[A] - p2[C]).norm();
                //double d2 = (p1[B] - p2[C]).norm();
                if (d2<d1) {
                    A = -1; // invalidate A
                    C = i; // change the invbinding
                } else {
                    B = -1; // invalidate A
                }
            }
        }
    }
//    double error;
//    std::cout<<"error :";
//    for (unsigned i=0;i<p1.size();i++) {
//        if (bindId[i]!=-1) {
//            error = (totaldistp1[i] - totaldistp2[bindId[i]])*(totaldistp1[i] - totaldistp2[bindId[i]]);
//            std::cout<< " " <<error << " ";
//        }
//    }
//    std::cout<<std::endl;

}

}

}

