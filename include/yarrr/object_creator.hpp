#pragma once

#include <yarrr/object.hpp>

namespace yarrr
{
class PhysicalParameters;

//todo: this should be moved to kind of meta magic
Object::Pointer create_ship();
Object::Pointer create_laser( const PhysicalParameters& ships_parameters );
}

