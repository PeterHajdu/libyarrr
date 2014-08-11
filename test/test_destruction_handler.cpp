#include <yarrr/destruction_handlers.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/object.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/delete_object.hpp>

#include <thectci/service_registry.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( delete_when_destroyed )
{
  void SetUp()
  {
    object.reset( new yarrr::Object() );
    delete_when_destroyed = new yarrr::DeleteWhenDestroyed();
    object->add_behavior( yarrr::ObjectBehavior::Pointer( delete_when_destroyed ) );

    deleted_object_id = 0;
    was_object_deleted = false;

    the::ctci::service< yarrr::EngineDispatcher >().register_listener< yarrr::DeleteObject >(
        [ this ]( const yarrr::DeleteObject& delete_object )
        {
          was_object_deleted = true;
          deleted_object_id = delete_object.object_id();
        } );
  }

  It( registers_itself_as_a_component )
  {
    AssertThat(
        &object->components.component< yarrr::DeleteWhenDestroyed >(),
        Equals( delete_when_destroyed ) );
  }

  It( sends_delete_object_when_it_is_destroyed )
  {
    object->dispatcher.dispatch( yarrr::ObjectDestroyed() );
    AssertThat( was_object_deleted, Equals( true ) );
    AssertThat( deleted_object_id, Equals( object->id ) );
  }

  bool was_object_deleted;
  yarrr::Object::Id deleted_object_id;
  yarrr::DeleteWhenDestroyed* delete_when_destroyed;
  yarrr::Object::Pointer object;
};

