#include <yarrr/destruction_handlers.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/object.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/delete_object.hpp>
#include <yarrr/lua_engine.hpp>

#include <thectci/service_registry.hpp>
#include <themodel/lua.hpp>

#include "test_services.hpp"

#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( call_when_destroyed )
{
  void SetUp()
  {
    object.reset( new yarrr::Object() );
    object->add_behavior( std::make_unique< yarrr::CallWhenDestroyed >(
          [ this ]( const yarrr::Object& object )
          {
            called_with = &object;
            was_called = true;
          } ) );

    was_called = false;
  }

  It( sends_delete_object_when_it_is_destroyed )
  {
    object->dispatcher.dispatch( yarrr::ObjectDestroyed() );
    AssertThat( was_called, Equals( true ) );
  }

  It( calls_the_callback_with_the_proper_object_as_parameter )
  {
    object->dispatcher.dispatch( yarrr::ObjectDestroyed() );
    AssertThat( called_with, Equals( object.get() ) );
  }

  It( wokrs_with_lua_function )
  {
    the::model::Lua lua;
    AssertThat( lua.run(
          "the_function_was_called = false\n"
          "function the_function()\n"
          "the_function_was_called = true\n"
          "end\n"
          ), Equals( true ) );

    yarrr::LuaFunction function( lua.state()[ "the_function" ] );
    object->add_behavior( std::make_unique< yarrr::CallWhenDestroyed >( function ) );
    object->dispatcher.dispatch( yarrr::ObjectDestroyed() );
    AssertThat( lua.assert_equals( "the_function_was_called", true ), Equals( true ) );
  }

  const yarrr::Object* called_with;
  yarrr::Object::Pointer object;
  bool was_called;
};

Describe( delete_when_destroyed )
{
  void SetUp()
  {
    object.reset( new yarrr::Object() );
    object->add_behavior( yarrr::delete_when_destroyed() );

    deleted_object_id = 0;
    was_object_deleted = false;

    the::ctci::service< yarrr::EngineDispatcher >().register_listener< yarrr::DeleteObject >(
        [ this ]( const yarrr::DeleteObject& delete_object )
        {
          was_object_deleted = true;
          deleted_object_id = delete_object.object_id();
        } );
  }

  void TearDown()
  {
    test::clean_engine_dispatcher();
  }

  It( sends_delete_object_when_it_is_destroyed )
  {
    object->dispatcher.dispatch( yarrr::ObjectDestroyed() );
    AssertThat( was_object_deleted, Equals( true ) );
    AssertThat( deleted_object_id, Equals( object->id() ) );
  }

  bool was_object_deleted;
  yarrr::Object::Id deleted_object_id;
  yarrr::Object::Pointer object;
};

Describe( respawn_when_destroyed )
{
  void destroy_object()
  {
    object->dispatcher.dispatch( yarrr::ObjectDestroyed() );
  }

  void SetUp()
  {
    object.reset( new yarrr::Object() );
    respawn_when_destroyed = new yarrr::RespawnWhenDestroyed();
    object->add_behavior( yarrr::ObjectBehavior::Pointer( respawn_when_destroyed ) );

    was_player_killed = false;
    the::ctci::service< yarrr::EngineDispatcher >().register_listener< yarrr::PlayerKilled >(
        [ this ]( const yarrr::PlayerKilled& player_killed )
        {
          was_player_killed = true;
          destroyed_object_id = player_killed.object_id;
        } );
    destroy_object();
  }

  void TearDown()
  {
    test::clean_engine_dispatcher();
  }

  It( registers_itself_as_a_component )
  {
    AssertThat(
        &object->components.component< yarrr::RespawnWhenDestroyed >(),
        Equals( respawn_when_destroyed ) );
  }

  It( should_send_player_killed_event_to_the_engine_when_object_is_destroyed )
  {
    AssertThat( was_player_killed, Equals( true ) );
  }

  It( should_send_the_object_id_in_the_player_killed_event )
  {
    AssertThat( destroyed_object_id, Equals( object->id() ) );
  }

  bool was_player_killed;
  yarrr::Object::Id destroyed_object_id;
  yarrr::RespawnWhenDestroyed* respawn_when_destroyed;
  yarrr::Object::Pointer object;
};

