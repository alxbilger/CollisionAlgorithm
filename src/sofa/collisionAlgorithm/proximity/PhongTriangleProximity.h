﻿//#pragma once

//#include <sofa/collisionAlgorithm/geometry/PhongTriangleGeometry.h>
//#include <sofa/collisionAlgorithm/proximity/TriangleProximity.h>
//#include <sofa/collisionAlgorithm/BaseProximity.h>

//namespace sofa
//{

//namespace collisionAlgorithm
//{

//template<class GEOMETRY>
//class PhongTriangleProximity : public TBaseProximity<GEOMETRY> {
//    friend class GEOMETRY::GEOMETRY;

//public :
//    typedef PhongTriangleProximity<GEOMETRY> PROXIMITY;

//    typedef typename GEOMETRY::TDataTypes DataTypes;
//    typedef typename DataTypes::VecCoord VecCoord;
//    typedef typename DataTypes::Coord Coord;
//    typedef typename DataTypes::Real Real;
//    typedef typename DataTypes::VecDeriv VecDeriv;
//    typedef typename DataTypes::MatrixDeriv MatrixDeriv;
//    typedef typename MatrixDeriv::RowIterator MatrixDerivRowIterator;
//    typedef core::objectmodel::Data< VecCoord >        DataVecCoord;
//    typedef core::objectmodel::Data< VecDeriv >        DataVecDeriv;
//    typedef core::objectmodel::Data< MatrixDeriv >     DataMatrixDeriv;
//    typedef sofa::core::behavior::MechanicalState<DataTypes> State;

//    PhongTriangleProximity(const GEOMETRY * geometry, unsigned eid,unsigned p1,unsigned p2,unsigned p3,double f1,double f2,double f3)
//    : TBaseProximity<GEOMETRY>(geometry)
//    , m_eid(eid) {
//        m_pid[0] = p1;
//        m_pid[1] = p2;
//        m_pid[2] = p3;

//        m_fact[0] = f1;
//        m_fact[1] = f2;
//        m_fact[2] = f3;
//    }

//    virtual defaulttype::Vector3 getPosition(core::VecCoordId v) const
//    {
//        const helper::ReadAccessor<DataVecCoord> & pos = this->m_geometry->getState()->read(v);

//        return pos[m_pid[0]] * m_fact[0] +
//               pos[m_pid[1]] * m_fact[1] +
//               pos[m_pid[2]] * m_fact[2];
//    }

//    virtual defaulttype::Vector3 getNormal() const {
//        return this->m_geometry->m_point_normals[m_pid[0]] * m_fact[0] +
//               this->m_geometry->m_point_normals[m_pid[1]] * m_fact[1] +
//               this->m_geometry->m_point_normals[m_pid[2]] * m_fact[2];
//    }

//    void addContributions(MatrixDerivRowIterator & it, const defaulttype::Vector3 & N) const {
//        it.addCol(m_pid[0], N * m_fact[0]);
//        it.addCol(m_pid[1], N * m_fact[1]);
//        it.addCol(m_pid[2], N * m_fact[2]);
//    }

//    static BaseProximity::SPtr project(const GEOMETRY * geometry, unsigned tid, const defaulttype::Vector3 & P) {
//        core::topology::BaseMeshTopology::Triangle triangle;
//        defaulttype::Vector3 factor;
//        geometry->projectLinear(tid, P, triangle, factor);

//        return BaseProximity::create<PROXIMITY>(geometry,tid,triangle[0],triangle[1],triangle[2],factor[0],factor[1],factor[2]);
//    }

//    static BaseProximity::SPtr center(const GEOMETRY * geometry, unsigned tid) {
//        const core::topology::BaseMeshTopology::Triangle & triangle = geometry->d_triangles.getValue()[tid];

//        return BaseProximity::create<PROXIMITY>(geometry,tid,triangle[0],triangle[1],triangle[2],0.3333,0.3333,0.3333);
//    }

//    static defaulttype::BoundingBox getBBox(const GEOMETRY * geometry, unsigned tid) {
//        return TriangleProximity<TriangleGeometry<DataTypes> >::getBBox(geometry,tid);
//    }


//protected:
//    unsigned m_eid;
//    unsigned m_pid[3];
//    double m_fact[3];

//};

//}

//}
