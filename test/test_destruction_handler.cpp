#include <yarrr/destruction_handlers.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/object.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( delete_when_destroyed )
{
  void SetUp()
  {
    object.reset( new yarrr::Object() );
    delete_when_destroyed = new yarrr::DeleteWhenDestroyed();
    object->add_behavior( yarrr::ObjectBehavior::Pointer( delete_when_destroyed ) );
  }

  It( registers_itself_as_a_component )
  {
    AssertThat(
        &object->components.component< yarrr::DeleteWhenDestroyed >(),
        Equals( delete_when_destroyed ) );
  }

  yarrr::DeleteWhenDestroyed* delete_when_destroyed;
  yarrr::Object::Pointer object;
};

