#pragma once

#include <sofa/collisionAlgorithm/BaseProximity.h>

namespace sofa
{

namespace collisionAlgorithm
{

class BaseElement {
public:
    typedef std::unique_ptr<BaseElement> UPtr;

    virtual BaseProximity::SPtr project(const defaulttype::Vector3 & P) const = 0;

    virtual BaseProximity::SPtr center() const = 0;

    virtual defaulttype::BoundingBox getBBox() const = 0;

};

class BaseElementIterator {
public:
    class UPtr : public std::unique_ptr<BaseElementIterator> {
    public:
        UPtr(BaseElementIterator * ptr) : std::unique_ptr<BaseElementIterator>(ptr) {}

        //we take the geometry as parameter for std::iterator compatibility i.e. it != m_geo->end();
        bool operator != (const BaseGeometry * /*geo*/) {
            return ! this->get()->end();
        }

        bool operator == (const BaseGeometry * /*geo*/) {
            return this->get()->end();
        }

        void operator++() {
            this->get()->next();
        }

        void operator++(int) {
            this->get()->next();
        }

        BaseElement::UPtr operator* () {
            return this->get()->element();
        }
    };


    static BaseElementIterator::UPtr empty() {
        class EmptyIterator : public BaseElementIterator {
        public:
            virtual bool end() const { return true; }

            virtual void next() {}

            virtual BaseElement::UPtr element() { return NULL; }

            virtual unsigned id() const { return 0; }
        };

        return UPtr(new EmptyIterator());
    }

    virtual bool end() const = 0;

    virtual void next() = 0;

    virtual BaseElement::UPtr element() = 0;

    virtual unsigned id() const = 0;
};

}

}
