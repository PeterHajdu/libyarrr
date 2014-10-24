#include <yarrr/coordinate_transformation.hpp>
#include <yarrr/types.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;


Describe(relative_coordinate)
{
  It( can_be_transformed_to_absolute_coordinate )
  {
    const yarrr::Coordinate absolute_coordinate(
        yarrr::transform_shape_relative_to_absolute_coordinate(
          relative_coordinate,
          center_of_mass_relative,
          center_of_object_absolute,
          orientation_of_object ) );

    AssertThat( absolute_coordinate, Equals( yarrr::Coordinate( 100_metres, 105_metres ) ) );
  }

  It( can_be_transformed_to_absolute_coordinate_from_center_of_mass_relative_coordinate )
  {
    const yarrr::Coordinate absolute_coordinate(
        yarrr::transform_center_of_mass_relative_to_absolute(
          relative_coordinate,
          center_of_object_absolute,
          orientation_of_object ) );

    AssertThat( absolute_coordinate, Equals( yarrr::Coordinate( 100_metres, 110_metres ) ) );
  }

  const yarrr::Coordinate relative_coordinate{ -10_metres, 0 };
  const yarrr::Coordinate center_of_mass_relative{ -5_metres, 0 };
  const yarrr::Coordinate center_of_object_absolute{ 100_metres, 100_metres };
  const yarrr::Angle orientation_of_object{ -90_degrees };
};

Describe(absolute_coordinate)
{
  It(can_be_transformed_to_relative_to_center_of_mass)
  {
    const yarrr::Coordinate relative_to_center_of_mass(
        yarrr::transform_absolute_to_relative_to_center_of_mass(
          absolute_coordinate,
          absolute_center_of_mass,
          orientation ) );
    AssertThat( relative_to_center_of_mass,
        Equals( yarrr::Coordinate{ 5_metres, 0 } ) );
  }

  const yarrr::Coordinate absolute_coordinate{ 0, 10_metres };
  const yarrr::Coordinate absolute_center_of_mass{ 0, 5_metres };
  const yarrr::Angle orientation{ 90_degrees };
};

