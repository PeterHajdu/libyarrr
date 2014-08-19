#include <yarrr/collider.hpp>
#include <yarrr/basic_behaviors.hpp>

namespace
{

int64_t length_square( const yarrr::Coordinate& coordinate )
{
  return
    coordinate.x * coordinate.x +
    coordinate.y * coordinate.y;
}

}

namespace yarrr
{

Collider::Collider( int layer, int16_t initial_integrity, int16_t caused_damage )
  : ObjectBehavior( do_not_syncronize )
  , m_physical_behavior( nullptr )
  , m_initial_integrity( initial_integrity )
  , m_caused_damage( caused_damage )
  , m_dispatcher( nullptr )
  , m_layer( layer )
{
}

void
Collider::register_to( Object& owner )
{
  owner.components.register_component( *this );
  assert( owner.components.has_component< PhysicalBehavior >() );
  m_physical_behavior = &owner.components.component< PhysicalBehavior >();
  m_dispatcher = &owner.dispatcher;

  reset_integrity();
}


ObjectBehavior::Pointer
Collider::clone() const
{
  return Pointer( new Collider( m_layer, m_initial_integrity, m_caused_damage ) );
}

void
Collider::collide_if_needed_with( Collider& other )
{
  assert( m_physical_behavior );

  const bool is_collider_on_the_same_layer( m_layer == other.m_layer );
  if ( is_collider_on_the_same_layer || is_collider_too_far( other ) )
  {
    return;
  }

  collide_with( other );
  other.collide_with( *this );
}

bool
Collider::is_collider_too_far( const Collider& other ) const
{
  Coordinate coordinate_difference(
      m_physical_behavior->physical_parameters.coordinate -
      other.m_physical_behavior->physical_parameters.coordinate );

  const Coordinate close_enough( 40, 40 );
  if ( length_square( coordinate_difference ) > length_square( close_enough ) )
  {
    return true;
  }

  return false;
}

void
Collider::collide_with( const Collider& other ) const
{
  current_integrity() -= other.m_caused_damage;

  if ( current_integrity() > 0 )
  {
    return;
  }

  m_dispatcher->dispatch( ObjectDestroyed() );
  reset_integrity();
}

void
Collider::reset_integrity() const
{
  current_integrity() = m_initial_integrity;
}

int16_t&
Collider::current_integrity() const
{
  assert( m_physical_behavior );
  return m_physical_behavior->physical_parameters.integrity;
}

}

