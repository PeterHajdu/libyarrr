#include <yarrr/object_factory.hpp>
#include <yarrr/lua_engine.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/object.hpp>
#include <yarrr/object_update.hpp>
#include <thectci/service_registry.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( an_object_factory )
{
  void SetUp()
  {
    object_factory.reset( new yarrr::ObjectFactory() );
    created_object_id = 0;
    object_factory->register_creator( key,
        [ this ]()
        {
          yarrr::Object::Pointer object( new yarrr::Object() );
          created_object_id = object->id();
          return object;
        } );
  }

  It( allows_factory_function_registration_by_type )
  {
    object_factory->register_creator( key,
        []()
        {
          return yarrr::Object::Pointer( nullptr );
        } );
  }

  It( creates_an_object_by_key )
  {
    yarrr::Object::Pointer object( object_factory->create_a( key ) );
    AssertThat( object->id(), Equals( created_object_id ) );
  }

  It( calls_the_correct_creator_in_case_of_multiple_keys )
  {
    bool was_another_creator_called{ false };
    object_factory->register_creator( "another key",
        [ &was_another_creator_called ]()
        {
          was_another_creator_called = true;
          return yarrr::Object::Pointer( nullptr );
        } );
    object_factory->create_a( key );
    AssertThat( was_another_creator_called, Equals( false ) );
    object_factory->create_a( "another key" );
    AssertThat( was_another_creator_called, Equals( true ) );
  }

  It( returns_nullptr_if_the_key_is_unknown )
  {
    auto object( object_factory->create_a( "cat" ) );
    AssertThat( object.get() == nullptr, Equals( true ) );
  }

  It( is_registered_as_a_service )
  {
    yarrr::ObjectFactory& object_factory( the::ctci::service< yarrr::ObjectFactory >() );
    (void)object_factory;
  }

  It( exports_factory_registration_to_the_lua_world )
  {
    the::model::Lua& lua( yarrr::LuaEngine::model() );
    lua.state().new_userdata< yarrr::Object >( "Object", "add_behavior", &yarrr::Object::add_behavior_clone );
    lua.state().new_userdata< yarrr::PhysicalBehavior >( "PhysicalBehavior" );

    AssertThat(
      lua.run(
        "function dogfood( new_object )\n"
        "  new_object:add_behavior( PhysicalBehavior.new() )\n"
        "end\n"
        "object_factory.register_factory( \"dogfood\", dogfood )\n" ),
      Equals( true ) );

    yarrr::Object::Pointer new_object( the::ctci::service< yarrr::ObjectFactory >().create_a( "dogfood" ) );
  }

  yarrr::Object::Id created_object_id;
  std::unique_ptr< yarrr::ObjectFactory > object_factory;
  const std::string key{ "duck" };
};

