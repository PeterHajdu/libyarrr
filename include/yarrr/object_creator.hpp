#pragma once

#include <yarrr/object.hpp>

namespace yarrr
{
class PhysicalParameters;
class ObjectIdentity;

//todo: this should be moved to kind of meta magic
Object::Pointer create_ship();

Object::Pointer create_laser(
    const PhysicalParameters&,
    const ObjectIdentity& );

Object::Pointer create_loot( PhysicalParameters new_physical_parameters, const ObjectBehavior& item );

}

