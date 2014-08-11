#include <yarrr/collider.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/object.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{
  const int16_t initial_integrity{ 100 };

  class ColliderBundle
  {
    public:
      ColliderBundle( int16_t caused_damage )
        : collider( new yarrr::Collider( initial_integrity, caused_damage ) )
        , physical_behavior( new yarrr::PhysicalBehavior() )
      {
        object.add_behavior( yarrr::ObjectBehavior::Pointer( physical_behavior ) );
        object.add_behavior( yarrr::ObjectBehavior::Pointer( collider ) );

        object.dispatcher.register_listener< yarrr::ObjectDestroyed >( std::bind(
              &ColliderBundle::handle_object_destroyed, this, std::placeholders::_1 ) );
      }

      ColliderBundle( ColliderBundle& other, int16_t caused_damage )
        : ColliderBundle( caused_damage )
      {
        physical_parameters() = other.physical_parameters();
      }

      yarrr::Object object;
      yarrr::Collider* const collider;

      int16_t integrity_decreased_with() const
      {
        return initial_integrity - physical_behavior->physical_parameters.integrity;
      }

      bool did_collide() const
      {
        return integrity_decreased_with() > 0;
      }

      yarrr::PhysicalParameters&
      physical_parameters()
      {
        return object.components.component< yarrr::PhysicalBehavior >().physical_parameters;
      }

      void move_far_enough()
      {
        physical_behavior->physical_parameters.coordinate += { 100000, 100000 };
      }

      bool was_object_destroyed_dispatched{ false };
      void handle_object_destroyed( const yarrr::ObjectDestroyed& )
      {
        was_object_destroyed_dispatched = true;
      }

    private:
      yarrr::PhysicalBehavior* const physical_behavior;
  };
}

Describe( a_collider )
{

  void SetUp()
  {
    collider_bundle.reset( new ColliderBundle( colliders_caused_damage ) );
    colliding_bundle.reset( new ColliderBundle( *collider_bundle, collidings_caused_damage ) );
    collider_too_far.reset( new ColliderBundle( *collider_bundle, colliders_caused_damage ) );
    collider_too_far->move_far_enough();
  }

  It( registers_itself_as_a_component )
  {
    AssertThat(
        &collider_bundle->object.components.component< yarrr::Collider >(),
        Equals( collider_bundle->collider ) );
  }

  void collide_with( ColliderBundle& other )
  {
    collider_bundle->collider->collide_if_needed( *other.collider );
  }

  It( decreases_integrity_with_the_other_colliders_caused_damage_when_colliding )
  {
    collide_with( *colliding_bundle );
    AssertThat( collider_bundle->integrity_decreased_with(), Equals( collidings_caused_damage ) )
    AssertThat( colliding_bundle->integrity_decreased_with(), Equals( colliders_caused_damage ) )
  }

  It( does_not_collide_if_objects_are_far_from_eachother )
  {
    collide_with( *collider_too_far );
    AssertThat( collider_bundle->did_collide(), Equals( false ) )
    AssertThat( colliding_bundle->did_collide(), Equals( false ) )
  }

  It( sets_integrity_to_its_default_value_when_registered )
  {
    AssertThat( collider_bundle->physical_parameters().integrity, Equals( initial_integrity ) );
  }

  void destroy_ship()
  {
    for ( int16_t integrity( initial_integrity ); integrity > 0; integrity -= collidings_caused_damage )
    {
      collide_with( *colliding_bundle );
    }
  }

  It( resets_integrity_if_object_is_destroyed )
  {
    destroy_ship();
    AssertThat( collider_bundle->physical_parameters().integrity, Equals( initial_integrity ) );
  }

  It( dispatches_object_destroyed_only_when_the_object_is_destroyed )
  {
    collide_with( *colliding_bundle );
    AssertThat( collider_bundle->was_object_destroyed_dispatched, Equals( false ) );
    destroy_ship();
    AssertThat( collider_bundle->was_object_destroyed_dispatched, Equals( true ) );
  }

  std::unique_ptr< ColliderBundle > collider_bundle;
  std::unique_ptr< ColliderBundle > colliding_bundle;
  std::unique_ptr< ColliderBundle > collider_too_far;

  const int16_t collidings_caused_damage{ 10 };
  const int16_t colliders_caused_damage{ 20 };
};

