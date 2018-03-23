#pragma once

#include <commonTypes.h>
#include <memory>
#include <map>
#include <vector>

namespace collisionAlgorithm {

class ConstraintElement;
class BaseGeometry;

class ConstraintProximity {
public :

    virtual Vector3 getPosition() const = 0;

    virtual Vector3 getFreePosition() const = 0;

    virtual Vector3 getNormal() const = 0;

    virtual std::map<unsigned,Vector3> getContribution(const Vector3 & N) = 0;

    virtual ConstraintElement * getElement() = 0;
};

typedef std::shared_ptr<ConstraintProximity> ConstraintProximityPtr;

class ConstraintElement {
public:

    //this function returns a vector with all the control points of the element
    virtual ConstraintProximityPtr getControlPoint(const int i) = 0;

    // return the number of control points
    virtual unsigned getNbControlPoints() = 0;

    //this function project the point P on the element and return the corresponding proximity
    virtual ConstraintProximityPtr project(Vector3 P) = 0;

    virtual void draw(const std::vector<Vector3> & pos) = 0;
};

typedef std::shared_ptr<ConstraintElement> ConstraintElementPtr;

class BaseGeometry : public BaseObject {
public:

    PortIn<Topology,REQUIRED> p_topology;
    PortIn<BaseObject> p_type;

    BaseGeometry()
    : p_topology(this)
    , p_type(this,"Any") {
        m_dirty = true;
    }

    void init() {
        m_elements.clear();
        createElements();
    }

    void beginStep() {
        if (m_dirty) {
            m_dirty = false;
            prepareDetection();
        }
    }

    void endStep() {
        m_dirty = true;
    }

    virtual void createElements() = 0;

    unsigned getNbElements() {
        beginStep();
        return m_elements.size();
    }

    static std::string getObjectType() {
        return std::string("Geometry");
    }

    ConstraintElementPtr getElement(unsigned i) const {
        return m_elements[i];
    }

    const std::vector<Vector3> & getPos() {
        return p_topology->p_state->get(TVecId::position);
    }

    const std::vector<Vector3> & getFreePos() {
        return p_topology->p_state->get(TVecId::freePosition);
    }

    virtual void prepareDetection() {}

    void draw(const VisualParams * vparams);

protected:
    std::vector<ConstraintElementPtr> m_elements;
    bool m_dirty;

};




} // namespace controller
