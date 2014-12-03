#pragma once

#include <yarrr/object.hpp>

namespace test
{
yarrr::Object::Pointer create_ship();
}

namespace yarrr
{
class PhysicalParameters;
class ObjectIdentity;

Object::Pointer create_laser(
    const PhysicalParameters&,
    const ObjectIdentity& );

Object::Pointer create_loot(
    PhysicalParameters,
    const ObjectBehavior& );

}

