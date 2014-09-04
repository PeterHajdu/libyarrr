#include <yarrr/collider.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/object.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_damage_causer )
{
  void SetUp()
  {
    object.reset( new yarrr::Object() );
    std::unique_ptr< yarrr::PhysicalBehavior > physical_behavior( new yarrr::PhysicalBehavior() );
    integrity = &physical_behavior->physical_parameters.integrity;
    object->add_behavior( yarrr::ObjectBehavior::Pointer( physical_behavior.release() ) );
    damage_causer = new yarrr::DamageCauser( original_integrity );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( damage_causer ) );

    was_object_destroyed_dispatched = false;
    object->dispatcher.register_listener< yarrr::ObjectDestroyed >(
        [ this ]( const yarrr::ObjectDestroyed& ){ was_object_destroyed_dispatched = true; } );
  }

  void collide()
  {
    object->dispatcher.dispatch( yarrr::Collide( dummy_object ) );
  }

  It( sets_integrity_to_its_default_value_when_registered )
  {
    AssertThat( *integrity, Equals( original_integrity ) );
  }

  It( decreases_integrity_with_ten_when_colliding )
  {
    collide();
    AssertThat( *integrity, Equals( original_integrity - 10 ) );
  }

  void destroy_object()
  {
    for ( int i( 0 ); i < original_integrity / 10; ++i )
    {
      AssertThat( was_object_destroyed_dispatched, Equals( false ) );
      collide();
    }
  }

  It( dispatches_object_destroyed_if_integrity_decreases_below_zero )
  {
    destroy_object();
    AssertThat( was_object_destroyed_dispatched, Equals( true ) );
  }

  It( resets_integrity_if_object_is_destroyed )
  {
    destroy_object();
    AssertThat( *integrity, Equals( original_integrity ) );
  }

  It( registers_itself_as_a_component )
  {
    AssertThat( object->components.has_component< yarrr::DamageCauser >(), Equals( true ) );
  }

  const int original_integrity{ 50 };

  yarrr::Object::Pointer object;
  yarrr::Object dummy_object;
  yarrr::DamageCauser* damage_causer;
  int16_t* integrity;

  bool was_object_destroyed_dispatched{ false };
};

