#pragma once

#include <sofa/collisionAlgorithm/BaseElement.h>
#include <sofa/collisionAlgorithm/BaseGeometryModifier.h>
#include <sofa/core/visual/VisualParams.h>
#include <sofa/simulation/AnimateBeginEvent.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/behavior/MechanicalState.h>
#include <sofa/core/BehaviorModel.h>
#include <memory>
#include <map>
#include <vector>
#include <GL/gl.h>

namespace sofa
{

namespace collisionAlgorithm
{

class BaseGeometry : public core::BehaviorModel
{
public:
    SOFA_CLASS(BaseGeometry,core::objectmodel::BaseObject);

    typedef Data<helper::vector<defaulttype::Vector3> > DataVecCoord;
    
    Data<defaulttype::Vector4> d_color;

    BaseGeometry()
    : d_color(initData(&d_color, defaulttype::Vector4(1,0,1,1), "color", "Color of the collision model"))
    , l_state(initLink("mstate", "link to state"))
    , l_normalHandler(initLink("normalHandler", "link to the normal handler"))
    {
        l_state.setPath("@.");
    }

    void bwdInit( ) override
    {
        setNormalHandler();

        prepareDetection();
    }

    void updatePosition(SReal ) override
    {
        prepareDetection();
    }

    std::size_t getNbElements() const
    {
        return m_elements.size();
    }

    const ConstraintElement* getElement(size_t i) const
    {
        return m_elements[i].get();
    }

    virtual sofa::core::behavior::MechanicalState<defaulttype::Vec3dTypes> * getState() const
    {
        return l_state.get();
    }

    void draw(const core::visual::VisualParams *vparams) override
    {
        if (! vparams->displayFlags().getShowCollisionModels())
            return;
        if (d_color.getValue()[3] == 0.0)
            return;

        glDisable(GL_LIGHTING);

        for(unsigned i=0;i<m_elements.size();i++)
        {
            m_elements[i]->draw(vparams);
        }
    }

    defaulttype::Vector3 getNormal(const unsigned elementID, const double* fact) const;

protected:

    virtual void setNormalHandler()
    {
        if(!l_normalHandler.get())
        {
            msg_warning(this) << "No normal handler given, finding one...";
            l_normalHandler.setPath("@.");
        }
        if(!l_normalHandler.get())
        {
            msg_warning(this) << "No normal handler found, creating a null one";
            SofaBaseNormalHandler::SPtr normalHandler = sofa::core::objectmodel::New<SofaBaseNormalHandler>(this);
            this->getContext()->addObject(normalHandler);
            l_normalHandler.set(normalHandler);
        }
    }

    std::vector<ConstraintElement::UPtr> m_elements;

    virtual void prepareDetection() {}

    core::objectmodel::SingleLink<BaseGeometry,sofa::core::behavior::MechanicalState<defaulttype::Vec3dTypes>,BaseLink::FLAG_STRONGLINK|BaseLink::FLAG_STOREPATH> l_state;
    core::objectmodel::SingleLink<BaseGeometry,SofaBaseNormalHandler,BaseLink::FLAG_STRONGLINK|BaseLink::FLAG_STOREPATH> l_normalHandler;

};

}

}
