#pragma once

namespace test
{
  class ColliderBundle
  {
    public:
      ColliderBundle( int layer )
        : collider( new yarrr::Collider( layer ) )
        , physical_behavior( new yarrr::PhysicalBehavior() )
      {
        object.add_behavior( yarrr::ObjectBehavior::Pointer( physical_behavior ) );
        object.add_behavior( yarrr::ObjectBehavior::Pointer( collider ) );
        object.dispatcher.register_listener< yarrr::Collide >( std::bind(
              &ColliderBundle::handle_collide, this, std::placeholders::_1 ) );
      }

      ColliderBundle( ColliderBundle& other, int layer )
        : ColliderBundle( layer )
      {
        physical_parameters() = other.physical_parameters();
      }

      yarrr::Object object;
      yarrr::Collider* const collider;

      yarrr::Object* collided_with{ nullptr };
      void handle_collide( const yarrr::Collide& collide )
      {
        collided_with = &collide.with;
      }

      bool did_collide() const
      {
        return collided_with != nullptr;
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

    private:
      yarrr::PhysicalBehavior* const physical_behavior;
  };
}

