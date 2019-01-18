﻿#pragma once

#include <sofa/collisionAlgorithm/geometry/BezierTriangleGeometry.h>
#include <sofa/collisionAlgorithm/proximity/TriangleProximity.h>

namespace sofa
{

namespace collisionAlgorithm
{

template<class GEOMETRY>
class BezierTriangleProximity : public TBaseProximity<GEOMETRY> {
    friend class GEOMETRY::GEOMETRY;

public:
    typedef BezierTriangleProximity<GEOMETRY> PROXIMITY;

    typedef typename GEOMETRY::TDataTypes DataTypes;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Real Real;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::MatrixDeriv MatrixDeriv;
    typedef typename DataTypes::Deriv Deriv1;
    typedef typename MatrixDeriv::RowIterator MatrixDerivRowIterator;
    typedef core::objectmodel::Data< VecCoord >        DataVecCoord;
    typedef core::objectmodel::Data< VecDeriv >        DataVecDeriv;
    typedef core::objectmodel::Data< MatrixDeriv >     DataMatrixDeriv;
    typedef sofa::core::behavior::MechanicalState<DataTypes> State;

    BezierTriangleProximity(const GEOMETRY * geometry, unsigned eid, unsigned p1,unsigned p2,unsigned p3,double f1,double f2,double f3)
    : TBaseProximity<GEOMETRY>(geometry)
    , m_eid(eid) {
        m_pid[0] = p1;
        m_pid[1] = p2;
        m_pid[2] = p3;

        m_fact[0] = f1;
        m_fact[1] = f2;
        m_fact[2] = f3;
    }

    ////Bezier triangle are computed according to :
    ////http://www.gamasutra.com/view/feature/131389/b%C3%A9zier_triangles_and_npatches.php?print=1
    defaulttype::Vector3 getPosition(core::VecCoordId v) const override
    {
        const typename GEOMETRY::BezierTriangleInfo & tbinfo = this->m_geometry->m_beziertriangle_info[m_eid];

        if(v == core::VecCoordId::position())
        {
            const helper::ReadAccessor<DataVecCoord> & x = this->m_geometry->getState()->read(v);

            const defaulttype::Vector3 & p300 = x[m_pid[2]];
            const defaulttype::Vector3 & p030 = x[m_pid[1]];
            const defaulttype::Vector3 & p003 = x[m_pid[0]];

            double fact_w = m_fact[2];
            double fact_u = m_fact[1];
            double fact_v = m_fact[0];

            return p300 *   fact_w*fact_w*fact_w +
                   p030 *   fact_u*fact_u*fact_u +
                   p003 *   fact_v*fact_v*fact_v +
                   tbinfo.p210 * 3*fact_w*fact_w*fact_u +
                   tbinfo.p120 * 3*fact_w*fact_u*fact_u +
                   tbinfo.p201 * 3*fact_w*fact_w*fact_v +
                   tbinfo.p021 * 3*fact_u*fact_u*fact_v +
                   tbinfo.p102 * 3*fact_w*fact_v*fact_v +
                   tbinfo.p012 * 3*fact_u*fact_v*fact_v +
                   tbinfo.p111 * 6*fact_w*fact_u*fact_v;
        }
        else if (v == core::VecCoordId::freePosition())
        {
            double fact_w = m_fact[2];
            double fact_u = m_fact[1];
            double fact_v = m_fact[0];

            const helper::ReadAccessor<DataVecCoord> & x = this->m_geometry->getState()->read(core::VecCoordId::freePosition());

            const defaulttype::Vector3 & p300_Free = x[m_pid[2]];
            const defaulttype::Vector3 & p030_Free = x[m_pid[1]];
            const defaulttype::Vector3 & p003_Free = x[m_pid[0]];

            const defaulttype::Vector3 & n200_Free = this->m_geometry->m_point_normals[m_pid[2]];
            const defaulttype::Vector3 & n020_Free = this->m_geometry->m_point_normals[m_pid[1]];
            const defaulttype::Vector3 & n002_Free = this->m_geometry->m_point_normals[m_pid[0]];

            double w12_free = dot(p030_Free - p300_Free,n200_Free);
            double w21_free = dot(p300_Free - p030_Free,n020_Free);
            double w23_free = dot(p003_Free - p030_Free,n020_Free);
            double w32_free = dot(p030_Free - p003_Free,n002_Free);
            double w31_free = dot(p300_Free - p003_Free,n002_Free);
            double w13_free = dot(p003_Free - p300_Free,n200_Free);

            const defaulttype::Vector3 & p210_Free = (p300_Free*2.0 + p030_Free - n200_Free * w12_free) / 3.0;
            const defaulttype::Vector3 & p120_Free = (p030_Free*2.0 + p300_Free - n020_Free * w21_free) / 3.0;

            const defaulttype::Vector3 & p021_Free = (p030_Free*2.0 + p003_Free - n020_Free * w23_free) / 3.0;
            const defaulttype::Vector3 & p012_Free = (p003_Free*2.0 + p030_Free - n002_Free * w32_free) / 3.0;

            const defaulttype::Vector3 & p102_Free = (p003_Free*2.0 + p300_Free - n002_Free * w31_free) / 3.0;
            const defaulttype::Vector3 & p201_Free = (p300_Free*2.0 + p003_Free - n200_Free * w13_free) / 3.0;

            const defaulttype::Vector3 & E_Free = (p210_Free+p120_Free+p102_Free+p201_Free+p021_Free+p012_Free) / 6.0;
            const defaulttype::Vector3 & V_Free = (p300_Free+p030_Free+p003_Free) / 3.0;
            const defaulttype::Vector3 & p111_Free =  E_Free + (E_Free-V_Free) / 2.0;

            return p300_Free *   fact_w*fact_w*fact_w +
                   p030_Free *   fact_u*fact_u*fact_u +
                   p003_Free *   fact_v*fact_v*fact_v +
                   p210_Free * 3*fact_w*fact_w*fact_u +
                   p120_Free * 3*fact_w*fact_u*fact_u +
                   p201_Free * 3*fact_w*fact_w*fact_v +
                   p021_Free * 3*fact_u*fact_u*fact_v +
                   p102_Free * 3*fact_w*fact_v*fact_v +
                   p012_Free * 3*fact_u*fact_v*fact_v +
                   p111_Free * 6*fact_w*fact_u*fact_v;
        }
        else
        {
            //TODO: different case ?
            return defaulttype::Vector3();
        }
    }

    defaulttype::Vector3 getNormal() const override
    {
        const typename GEOMETRY::BezierTriangleInfo & tbinfo = this->m_geometry->m_beziertriangle_info[m_eid];

        const defaulttype::Vector3 &n200 = this->m_geometry->m_point_normals[m_pid[2]];
        const defaulttype::Vector3 &n020 = this->m_geometry->m_point_normals[m_pid[1]];
        const defaulttype::Vector3 &n002 = this->m_geometry->m_point_normals[m_pid[0]];

        double fact_w = m_fact[2];
        double fact_u = m_fact[1];
        double fact_v = m_fact[0];

        defaulttype::Vector3 normal = n200 * fact_w*fact_w +
                                      n020 * fact_u*fact_u +
                                      n002 * fact_v*fact_v +
                                      tbinfo.n110 * fact_w*fact_u +
                                      tbinfo.n011 * fact_u*fact_v +
                                      tbinfo.n101 * fact_w*fact_v;

        defaulttype::Vector3 N1 = normal;
        N1.normalize();

        return N1;
    }

    void addContributions(MatrixDerivRowIterator & it, const defaulttype::Vector3 & N) const {
        it.addCol(m_pid[0], N * m_fact[0]);
        it.addCol(m_pid[1], N * m_fact[1]);
        it.addCol(m_pid[2], N * m_fact[2]);
    }

    static BaseProximity::SPtr project(const GEOMETRY * geometry, unsigned tid, const defaulttype::Vector3 & P) {
        core::topology::BaseMeshTopology::Triangle triangle;
        defaulttype::Vector3 factor;
        geometry->projectBezier(tid, P, triangle, factor);

        return BaseProximity::create<PROXIMITY>(geometry,tid,triangle[0],triangle[1],triangle[2],factor[0],factor[1],factor[2]);
    }

    static BaseProximity::SPtr center(const GEOMETRY * geometry, unsigned tid) {
        const core::topology::BaseMeshTopology::Triangle & triangle = geometry->d_triangles.getValue()[tid];

        return BaseProximity::create<PROXIMITY>(geometry,tid,triangle[0],triangle[1],triangle[2],0.3333,0.3333,0.3333);
    }

    static defaulttype::BoundingBox getBBox(const GEOMETRY * geometry, unsigned tid) {
        return TriangleProximity<TriangleGeometry<DataTypes>>::getBBox(geometry,tid);
    }

protected:
    unsigned m_eid;
    unsigned m_pid[3];
    double m_fact[3];
};

}

}
