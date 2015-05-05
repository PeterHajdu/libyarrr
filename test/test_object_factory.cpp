#include <yarrr/object_factory.hpp>
#include <yarrr/lua_engine.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/object.hpp>
#include <yarrr/object_decorator.hpp>
#include <yarrr/object_update.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/object_created.hpp>
#include <yarrr/destruction_handlers.hpp>
#include <thectci/service_registry.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( an_object_factory )
{
  void register_creator_with_name( const std::string& name )
  {
    object_factory->register_creator( name,
        [ this ]()
        {
          yarrr::Object::Pointer object( new yarrr::Object() );
          created_object_id = object->id();
          return object;
        } );
  }

  void set_up_lua()
  {
    lua = &yarrr::LuaEngine::model();
    AssertThat(
      lua->run(
        "function an_object_key( new_object )\n"
        "end\n"
        "print(123)\n"
        "object_factory.register_factory( \"" + key + "\", an_object_key )\n"
        "print(123)\n"
        ""
        "was_decorator_called = false\n"
        "function decorator( new_object )\n"
        "  was_decorator_called = true\n"
        "end\n" ),
      Equals( true ) );
  }

  void SetUp()
  {
    object_factory.reset( new yarrr::ObjectFactory() );
    created_object_id = 0;
    register_creator_with_name( key );
    set_up_lua();

    was_object_created_dispatched = false;
    the::ctci::service< yarrr::EngineDispatcher >().register_listener< yarrr::ObjectCreated >(
        [ this ]( const yarrr::ObjectCreated& created ){
          was_object_created_dispatched = true;
          created_object = std::move( created.object );
        } );
  }

  void TearDown()
  {
    the::ctci::service< yarrr::EngineDispatcher >().clear();
  }

  It( allows_factory_function_registration_by_type )
  {
    object_factory->register_creator( key,
        []()
        {
          return yarrr::Object::Pointer( nullptr );
        } );
  }

  It( handles_null_objects_returned_from_a_creator )
  {
    object_factory->register_creator( key,
        []()
        {
          return yarrr::Object::Pointer( nullptr );
        } );
    object_factory->create_a( key );
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
          return std::make_unique< yarrr::Object >();
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
    lua->state().new_userdata< yarrr::ObjectDecorator, yarrr::Object& >( "Object",
      "add_behavior", &yarrr::ObjectDecorator::add_behavior_clone );

    lua->state().new_userdata< yarrr::PhysicalBehavior >( "PhysicalBehavior" );

    AssertThat(
      lua->run(
        "function dogfood( new_object )\n"
        "  new_object:add_behavior( PhysicalBehavior.new() )\n"
        "end\n"
        "object_factory.register_factory( \"dogfood\", dogfood )\n" ),
      Equals( true ) );

    yarrr::Object::Pointer new_object( the::ctci::service< yarrr::ObjectFactory >().create_a( "dogfood" ) );
    AssertThat( new_object.get() != nullptr, Equals( true ) );
    AssertThat( yarrr::has_component< yarrr::PhysicalBehavior >( *new_object ), Equals( true ) );
  }

  It( can_return_registered_object_names )
  {
    const std::string another_ship_name{ "another_ship_name" };
    register_creator_with_name( another_ship_name );
    AssertThat( object_factory->objects(), Contains( key ) );
    AssertThat( object_factory->objects(), Contains( another_ship_name ) );
  }


  void create_object_from_lua()
  {
    AssertThat(
      lua->run(
        "object_factory.create_object( \"" + key + "\", decorator )\n" ),
      Equals( true ) );
  }


  It( sends_lua_created_objects_to_the_engine_dispatcher )
  {
    create_object_from_lua();
    AssertThat( was_object_created_dispatched, Equals( true ) );
  }


  It( calls_the_lua_decorator_function_when_called )
  {
    create_object_from_lua();
    AssertThat( lua->assert_equals( "was_decorator_called", true ), Equals( true ) );
  }


  the::model::Lua* lua;
  yarrr::Object::Id created_object_id;
  std::unique_ptr< yarrr::ObjectFactory > object_factory;
  const std::string key{ "duck" };

  yarrr::Object::Pointer created_object;

  bool was_object_created_dispatched;
};

