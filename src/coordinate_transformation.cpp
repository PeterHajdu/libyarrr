#include <yarrr/coordinate_transformation.hpp>

namespace yarrr
{

Coordinate
transform_shape_relative_to_absolute_coordinate(
    const Coordinate& relative_coordinate,
    const Coordinate& center_of_mass_relative,
    const Coordinate& center_of_object_absolute,
    const Angle& orientation_of_object )
{
  return transform_center_of_mass_relative_to_absolute(
      relative_coordinate - center_of_mass_relative,
      center_of_object_absolute,
      orientation_of_object );
}

Coordinate
transform_center_of_mass_relative_to_absolute(
    const Coordinate& relative_to_center_of_mass_coordinate,
    const Coordinate& center_of_object_absolute,
    const Angle& orientation_of_object )
{
  Coordinate absolute_coordinate( relative_to_center_of_mass_coordinate );
  rotate( absolute_coordinate, orientation_of_object );
  absolute_coordinate += center_of_object_absolute;
  return absolute_coordinate;
}

Coordinate
transform_absolute_to_relative_to_center_of_mass(
  const Coordinate& absolute_coordinate,
  const Coordinate& absolute_center_of_mass,
  const Angle& orientation )
{
  Coordinate relative( absolute_coordinate - absolute_center_of_mass );
  yarrr::rotate( relative, -orientation );
  return relative;
}

}

