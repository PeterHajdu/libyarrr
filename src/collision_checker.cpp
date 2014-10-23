#include <yarrr/collision_checker.hpp>
#include <yarrr/object.hpp>
#include <yarrr/shape_behavior.hpp>
#include <yarrr/basic_behaviors.hpp>

namespace
{

bool has_shape( const yarrr::Object& object )
{
  return yarrr::has_component< yarrr::ShapeBehavior >( object );
}

yarrr::Coordinate::type
collision_radius_of( const yarrr::Object& object )
{
  return yarrr::component_of< yarrr::ShapeBehavior >( object ).shape.radius();
}

yarrr::Coordinate
center_of( const yarrr::Object& object )
{
  return yarrr::component_of< yarrr::PhysicalBehavior >( object ).physical_parameters.coordinate;
}

bool
does_point_and_shaped_collide( const yarrr::Object& shapeless, const yarrr::Object& shaped )
{
  const yarrr::Coordinate center_of_mass_difference( center_of( shaped ) - center_of( shapeless ) );
  return yarrr::component_of< yarrr::ShapeBehavior >( shaped ).
    shape.does_contain( center_of_mass_difference );
}

}

namespace yarrr
{

bool does_collide( const yarrr::Object& a, const yarrr::Object& b )
{
  const bool is_shapeless_a( !has_shape( a ) );
  const bool is_shapeless_b( !has_shape( b ) );

  const bool both_shapeless( is_shapeless_a && is_shapeless_b );
  if ( both_shapeless )
  {
    return false;
  }

  const bool both_shaped( !is_shapeless_a && !is_shapeless_b );
  if ( both_shaped )
  {
    const auto distance_of_centers( yarrr::length_of( center_of( a ) - center_of( b ) ) );
    return distance_of_centers < collision_radius_of( a ) + collision_radius_of( b );
  }

  return is_shapeless_a ?
    does_point_and_shaped_collide( a, b ) :
    does_point_and_shaped_collide( b, a );
}

}

