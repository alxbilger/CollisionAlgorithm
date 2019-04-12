#pragma once

#include <sofa/core/collision/Pipeline.h>
#include <sofa/collisionAlgorithm/BaseGeometry.h>

namespace sofa {

namespace collisionAlgorithm {

/*!
 * \brief The BroadPhase class defines an interface for the collision detection broad phase
 */
class BroadPhase : public core::objectmodel::BaseObject
{
public:
    SOFA_ABSTRACT_CLASS(BroadPhase,core::objectmodel::BaseObject);

    Data<defaulttype::Vector4> d_color;
    core::objectmodel::SingleLink<BroadPhase,BaseGeometry,BaseLink::FLAG_STRONGLINK|BaseLink::FLAG_STOREPATH> l_geometry;

    /*!
     * \brief BroadPhase Constructor
     */
    BroadPhase()
    : d_color(initData(&d_color, defaulttype::Vector4(1,0,1,1), "color", "Color of the collision model"))
    , l_geometry(initLink("geometry", "link to state")) {}

    /*!
     * \brief ~BroadPhase destructor
     */
    virtual ~BroadPhase() {
        if (l_geometry != NULL) l_geometry->setBroadPhase(NULL);
    }

    void init( ) override {
        if (l_geometry != NULL) {
            sout << "Register to geometry " << l_geometry->getName() << sendl;
            l_geometry->setBroadPhase(this);
        }
        serr << "No geometry found" << sendl;
    }

    /*!
     * \brief prepareDetection virtual method to implement
     * detection pre-processing
     */
    virtual void prepareDetection() = 0;

    /*!
     * \brief getBoxSize
     * \return bounding box size in a vec3i
     */
    virtual defaulttype::Vec3i getBoxSize() const = 0;

    /*!
     * \brief getBoxCoord
     * \param P : point in space
     * \return the box's coordinates (vec3i) containing point P
     */
    virtual defaulttype::Vec3i getBoxCoord(const defaulttype::Vector3 & P) const = 0;

    virtual void getElementSet(unsigned cx,unsigned cy, unsigned cz, std::set<unsigned> & selectElements) const = 0;

    void update(double time) {
        if (m_updateTime < time) {
            m_updateTime = time;
            prepareDetection();
        }
        if (l_geometry) l_geometry->update(time); // make sure the geometry is updated on the same time
    }

protected:

    double m_updateTime;

};


}

}
