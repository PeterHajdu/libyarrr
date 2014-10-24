#pragma once

#include <yarrr/types.hpp>

namespace yarrr
{

Coordinate transform_shape_relative_to_absolute_coordinate(
    const Coordinate& relative_coordinate,
    const Coordinate& center_of_mass_relative,
    const Coordinate& center_of_object_absolute,
    const Angle& orientation_of_object );

Coordinate transform_center_of_mass_relative_to_absolute(
    const Coordinate& relative_to_center_of_mass_coordinate,
    const Coordinate& center_of_object_absolute,
    const Angle& orientation_of_object );

Coordinate transform_absolute_to_relative_to_center_of_mass(
  const Coordinate& absolute_coordinate,
  const Coordinate& absolute_center_of_mass,
  const Angle& orientation );

}

