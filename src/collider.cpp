#include <yarrr/collider.hpp>
#include <yarrr/collision_checker.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/object.hpp>
#include <yarrr/object_identity.hpp>
#include <yarrr/object_destroyed.hpp>

namespace yarrr
{
Collide::Collide( Object& the_other_object )
  : with( the_other_object )
{
}

Collider::Collider( int layer )
  : ObjectBehavior( do_not_synchronize() )
  , m_physical_behavior( nullptr )
  , m_layer( layer )
{
}


void
Collider::do_register_to( Object& owner )
{
  assert( owner.components.has_component< PhysicalBehavior >() );
  m_physical_behavior = &owner.components.component< PhysicalBehavior >();
}


ObjectBehavior::Pointer
Collider::clone() const
{
  return Pointer( new Collider( m_layer ) );
}

void
Collider::collide_if_needed_with( Collider& other )
{
  assert( m_physical_behavior );

  const bool is_collider_on_the_same_layer( m_layer == other.m_layer );
  if ( is_collider_on_the_same_layer ||
       is_collider_too_far( other ) ||
       is_collider_from_the_same_captain( other ) )
  {
    return;
  }

  collide_with( other );
  other.collide_with( *this );
}

bool
Collider::is_collider_too_far( const Collider& other ) const
{
  return !does_collide( *m_object, *other.m_object );
}

bool
Collider::is_collider_from_the_same_captain( const Collider& other ) const
{
  const bool both_has_identity{
    has_component< ObjectIdentity >( *m_object ) &&
    has_component< ObjectIdentity >( *other.m_object ) };

  if ( !both_has_identity )
  {
    return false;
  }

  const bool has_same_captain{
    component_of< ObjectIdentity >( *m_object ).captain() ==
    component_of< ObjectIdentity >( *other.m_object ).captain() };

  if ( !has_same_captain )
  {
    return false;
  }

  return true;
}

void
Collider::collide_with( const Collider& other ) const
{
  m_object->dispatcher.dispatch( Collide( *other.m_object ) );
}


DamageCauser::DamageCauser( int initial_integrity )
  : ObjectBehavior( do_not_synchronize() )
  , m_initial_integrity( initial_integrity )
  , m_integrity( nullptr )
{
}


void
DamageCauser::do_register_to( Object& owner )
{
  assert( owner.components.has_component< PhysicalBehavior >() );
  m_integrity = &owner.components.component< PhysicalBehavior >().physical_parameters.integrity;
  owner.dispatcher.register_listener< Collide >(
      [ this ]( const Collide& collide ){ handle_collision( collide ); } );
  reset_integrity();
}


ObjectBehavior::Pointer
DamageCauser::clone() const
{
  return Pointer( new DamageCauser( m_initial_integrity ) );
}


void
DamageCauser::handle_collision( const Collide& ) const
{
  assert( m_integrity );
  (*m_integrity) -= 10;

  const bool destroyed( *m_integrity <= 0 );
  if ( destroyed )
  {
    reset_integrity();
    m_object->dispatcher.dispatch( yarrr::ObjectDestroyed() );
  }
}

void
DamageCauser::reset_integrity() const
{
  *m_integrity = m_initial_integrity;
}

}

