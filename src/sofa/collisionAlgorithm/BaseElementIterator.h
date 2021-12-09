#pragma once

#include <sofa/collisionAlgorithm/BaseProximity.h>

namespace sofa
{

namespace collisionAlgorithm
{

class BaseGeometry;

/*!
 * \brief The BaseElementIterator class defines an abstract iterator class for BaseElements
 */
class BaseElementIterator {
    typedef BaseProximity::Index Index;
public:

    ///defines a unique pointer iterator of baseElements
    class SPtr : public std::shared_ptr<BaseElementIterator> {
    public:
        SPtr(BaseElementIterator * ptr) : std::shared_ptr<BaseElementIterator>(ptr) {}

        //we take the geometry as parameter for std::iterator compatibility i.e. it != m_geo->end();
        bool operator != (const BaseGeometry * ) {
            return ! this->get()->end();
        }

        //we take the geometry as parameter for std::iterator compatibility i.e. it != m_geo->end();
        bool operator== (const BaseGeometry * ) {
            return this->get()->end();
        }

        void operator++() {
            this->get()->next();
        }

        void operator++(int) {
            this->get()->next();
        }

        const BaseElementIterator * operator* () {
            return this->get();
        }

        const BaseElementIterator * operator* () const {
            return this->get();
        }
    };

    //This is necessary to have the correct behavior with the delete !
    virtual ~BaseElementIterator() = default;

    ///returns a new EmptyIterator
    static BaseElementIterator::SPtr empty() {
        class EmptyIterator : public BaseElementIterator {
        public:
            virtual bool end() const { return true; }

            virtual void next() {}

            virtual BaseProximity::SPtr createProximity(CONTROL_POINT /*id*/ = CONTROL_DEFAULT) const override { return NULL; }

            virtual Index elementSize() const { return 0; }

            virtual Index id() const { return 0; }
        };

        return SPtr(new EmptyIterator());
    }

    virtual bool end() const = 0;

    virtual void next() = 0;

    virtual BaseProximity::SPtr createProximity(CONTROL_POINT id = CONTROL_DEFAULT) const = 0;

    virtual Index elementSize() const = 0;

    virtual Index id() const = 0;

};

}

}
