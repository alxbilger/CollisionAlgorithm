#pragma once

#include <sofa/collisionAlgorithm/BaseGeometry.h>
#include <sofa/collisionAlgorithm/proximity/TriangleProximity.h>
#include <sofa/collisionAlgorithm/elements/TriangleElement.h>
#include <sofa/collisionAlgorithm/toolbox/TriangleToolBox.h>
#include <sofa/collisionAlgorithm/proximity/TopologyProximity.h>

namespace sofa::collisionAlgorithm {

template<class DataTypes>
class TriangleGeometry : public TBaseGeometry<DataTypes>/*, public TriangleProximityCreator*/ {
public:
    typedef DataTypes TDataTypes;
    typedef TriangleElement ELEMENT;
    typedef TriangleGeometry<DataTypes> GEOMETRY;
    typedef TBaseGeometry<DataTypes> Inherit;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef core::objectmodel::Data< VecCoord >        DataVecCoord;
    typedef std::function<BaseProximity::SPtr(const TriangleElement * elmt, double f0,double f1,double f2)> ProximityCreatorFunc;

    SOFA_CLASS(GEOMETRY,Inherit);

    core::objectmodel::SingleLink<GEOMETRY,core::topology::BaseMeshTopology,BaseLink::FLAG_STRONGLINK|BaseLink::FLAG_STOREPATH> l_topology;

    TriangleGeometry()
    : l_topology(initLink("topology", "link to topology")) {
        l_topology.setPath("@.");
    }

//    type::Vector3 getPosition(unsigned pid) override {
//        const helper::ReadAccessor<DataVecCoord> & pos = this->getState()->read(core::VecCoordId::position());
//        return pos[pid];
//    }

    void init() {
        //default proximity creator
        this->m_topoProx.clear();
        for (unsigned j=0; j<this->getState()->getSize(); j++) {
            this->m_topoProx.push_back(TBaseProximity<DataTypes>::template create<TopologyProximity<DataTypes>>(this->getState(), j));
        }

        m_elements.clear();
        for (unsigned i=0;i<this->l_topology->getNbTriangles();i++) {
            auto tri = this->l_topology->getTriangle(i);
            auto elmt = BaseElement::create<TriangleElement>(this->m_topoProx[tri[0]],this->m_topoProx[tri[1]],this->m_topoProx[tri[2]]);
            m_elements.push_back(elmt);
        }

        prepareDetection();
    }

    void prepareDetection() override {
        const helper::ReadAccessor<DataVecCoord> & pos = this->getState()->read(core::VecCoordId::position());
        for (unsigned i=0;i<m_elements.size();i++) m_elements[i]->update(pos.ref());
    }

    inline ElementIterator::SPtr begin() const override {
        return ElementIterator::SPtr(new TDefaultElementIteratorSPtr(m_elements));
    }


    void setCreateProximity(ProximityCreatorFunc f) {
        for (unsigned i=0; i<m_elements.size(); i++) {
            m_elements[i]->setCreateProximity(f);
        }
    }

    inline std::vector<TriangleElement::SPtr> & getElements() {
        return m_elements;
    }


private:
    std::vector<TriangleElement::SPtr> m_elements;
};

}

