#pragma once

#include <sofa/collisionAlgorithm/BaseGeometry.h>
#include <sofa/core/collision/Pipeline.h>
#include <sofa/collisionAlgorithm/data/DataDetectionOutput.h>
#include <sofa/simulation/UpdateMappingEndEvent.h>
#include <sofa/simulation/CollisionEndEvent.h>

namespace sofa
{

namespace collisionAlgorithm
{

/*!
 * \brief The BaseFilter class provides an interface to create proximity filter components
 */
class BaseFilter : public sofa::core::objectmodel::BaseObject {
public:
    SOFA_ABSTRACT_CLASS(BaseFilter, sofa::core::objectmodel::BaseObject);

    /*!
     * \brief accept is a pure virtual method to implement
     * \param p1 : source proximity
     * \param p2 : destination proximity
     * \return bool : if the filter accepted the proximities
     */
    virtual bool accept(const BaseProximity::SPtr & p1,const BaseProximity::SPtr & p2) const = 0;
};

/*!
 * \class BaseAlgorithm
 * \brief The BaseAlgorithm abstract class defines an interface of
 * algorithms to be wrapped in sofa components
 */
class BaseAlgorithm : public sofa::core::objectmodel::BaseObject
{
public :

    SOFA_ABSTRACT_CLASS(BaseAlgorithm, sofa::core::objectmodel::BaseObject);

    core::objectmodel::MultiLink<BaseAlgorithm,BaseFilter,BaseLink::FLAG_STRONGLINK|BaseLink::FLAG_STOREPATH> l_filters;
    Data<bool> drawCollision ;
    Data<DetectionOutput> d_output;

    /*!
     * \brief BaseAlgorithm Constructor
     */
    BaseAlgorithm()
    : l_filters(initLink("filters","list of filters"))
    , drawCollision (initData(&drawCollision, false, "drawcollision", "draw collision"))
    , d_output(initData(&d_output,"output", "output of the collision detection")) {
        this->f_listening.setValue(true);
    }

    /*!
     * \brief acceptFilter loops through all filters linked to the component
     * \param pfrom : source proximity
     * \param pdest : destination proximity
     * \return True if all filters accept proximities, otherwise False
     */
    bool acceptFilter(const BaseProximity::SPtr & pfrom,const BaseProximity::SPtr & pdest) const {
        for (auto itfilter = l_filters.begin();itfilter != l_filters.end();itfilter++) {
            const BaseFilter * filter = (*itfilter);
            if (filter == NULL) continue;
            if (! filter->accept(pfrom,pdest)) return false;
        }
        return true;
    }


protected:

    virtual void doDetection() = 0;

    void draw(const core::visual::VisualParams* /*vparams*/) {
        if (drawCollision.getValue()) {
            glDisable(GL_LIGHTING);
            glColor4f(0,1,0,1);

            glBegin(GL_LINES);
            DetectionOutput output = d_output.getValue() ;
            for (unsigned i=0;i<output.size();i++) {
                glVertex3dv(output[i].first->getPosition().data());
                glVertex3dv(output[i].second->getPosition().data());
            }
            glEnd();
        }
    }

    void handleEvent(sofa::core::objectmodel::Event *event) {
        if (! dynamic_cast<sofa::simulation::CollisionBeginEvent*>(event)) return;

        doDetection();
    }
};

}

}
