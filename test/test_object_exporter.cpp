#include <yarrr/object_exporter.hpp>
#include <yarrr/object_container.hpp>
#include <yarrr/lua_engine.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(an_object_exporter)
{
  void add_new_object()
  {
    auto object( yarrr::Object::create() );
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::PhysicalBehavior() ) );
    object_ids.push_back( object->id() );
    container->add_object( std::move( object ) );
  }

  void set_up_object_container()
  {
    container.reset( new yarrr::ObjectContainer() );
    object_ids.clear();
    for ( size_t i( 0 ); i < number_of_objects; ++i )
    {
      add_new_object();
    }
  }

  void SetUp()
  {
    set_up_object_container();
    lua.reset( new sol::state() );
    lua->open_libraries( sol::lib::base );

    const yarrr::ObjectContainer& const_container( *container );
    exporter.reset( new yarrr::ObjectExporter( const_container, *lua ) );
  }

  It( exposes_objects_table_to_the_lua_world )
  {
    lua->script( "assert( objects )" );
  }

  void assert_every_object_is_exported()
  {
    for ( const auto& id : object_ids )
    {
      lua->script( std::string( "assert( objects[ \"" ) + std::to_string( id ) + "\" ] )" );
    }
  }

  It( exports_all_of_the_objects )
  {
    assert_every_object_is_exported();
  }

  It( adds_new_objects_when_refreshed )
  {
    add_new_object();
    exporter->refresh();
    assert_every_object_is_exported();
  }

  It( removes_deleted_objects_when_refreshed )
  {
    const yarrr::Object::Id removed_object( object_ids.back() );
    container->delete_object( removed_object );
    exporter->refresh();
    lua->script( std::string( "assert( not objects[ \"" ) + std::to_string( removed_object ) + "\" ] )" );
  }

  void assert_every_object_has_member( const std::string& member )
  {
    for ( const auto& id : object_ids )
    {
      lua->script( std::string( "assert( objects[ \"" ) + std::to_string( id ) + "\" ]." + member + " )" );
    }
  }

  It( exports_object_models )
  {
    assert_every_object_has_member( "coordinate.x" );
    assert_every_object_has_member( "coordinate.y" );
    assert_every_object_has_member( "velocity.x" );
    assert_every_object_has_member( "velocity.y" );
    assert_every_object_has_member( "orientation" );
    assert_every_object_has_member( "angular_velocity" );
  }

  const size_t number_of_objects{ 3 };
  std::unique_ptr< yarrr::ObjectContainer > container;
  std::unique_ptr< yarrr::ObjectExporter > exporter;
  std::unique_ptr< sol::state > lua;
  std::vector< yarrr::Object::Id > object_ids;
};


Describe( an_object_model )
{
  void SetUp()
  {
    parameters.coordinate = position;
    parameters.velocity = position;
    parameters.orientation = orientation;
    parameters.angular_velocity = angular_velocity;

    object = yarrr::Object::create();
    object->add_behavior( yarrr::ObjectBehavior::Pointer( new yarrr::PhysicalBehavior( parameters ) ) );

    lua.reset( new sol::state() );
    lua->open_libraries( sol::lib::base );

    id = std::to_string( object->id() );
    model.reset( new yarrr::ObjectModel( *object, *lua ) );

    objects = lua->create_table();
    objects[ id ] = model->table();
    (*lua)[ "objects" ] = objects;
  }

  It( can_be_added_to_a_lua_table )
  {
    lua->script( std::string( "assert( objects[ \"" ) + id + "\" ] )");
  }

  void assert_has_member( const std::string& member )
  {
    try
    {
      lua->script( std::string( "assert( objects[ \"" ) + id + "\" ]." + member + " )");
    }
    catch( std::exception& e )
    {
      std::cout << e.what() << std::endl;
      throw e;
    }
  }

  void assert_member_equals( const std::string& member, const std::string& value )
  {
    try
    {
      lua->script( std::string( "print( objects[ \"" ) + id + "\" ]." + member + ")" );
      lua->script( std::string( "assert( objects[ \"" ) + id + "\" ]." + member + " == " + value + " )");
    }
    catch( std::exception& e )
    {
      std::cout << e.what() << std::endl;
      throw e;
    }
  }

  It( exports_the_objects_coordinate_from_physical_parameters )
  {
    assert_has_member( "coordinate" );
    assert_has_member( "coordinate.x" );
    assert_has_member( "coordinate.y" );

    assert_member_equals( "coordinate.x", std::to_string( position_in_metres.x ) );
    assert_member_equals( "coordinate.y", std::to_string( position_in_metres.y ) );
  }

  It( exports_the_objects_velocity_from_physical_parameters )
  {
    assert_has_member( "velocity" );
    assert_has_member( "velocity.x" );
    assert_has_member( "velocity.y" );

    assert_member_equals( "velocity.x", std::to_string( velocity_in_metres.x ) );
    assert_member_equals( "velocity.y", std::to_string( velocity_in_metres.y ) );
  }

  It( exports_the_objects_orientation )
  {
    assert_member_equals( "orientation", std::to_string( orientation_in_degrees ) );
  }

  It( exports_the_objects_angular_velocity )
  {
    assert_member_equals( "angular_velocity", std::to_string( angular_velocity_in_degrees ) );
  }

  const yarrr::Angle orientation{ 1823839 };
  const double orientation_in_degrees{ yarrr::hiplon_to_degrees( orientation ) };

  const yarrr::Angle angular_velocity{ 18378239 };
  const double angular_velocity_in_degrees{ yarrr::hiplon_to_degrees( angular_velocity ) };

  const yarrr::Coordinate position{ 129833, -20000 };
  const yarrr::Coordinate position_in_metres{ yarrr::huplons_to_metres( position ) };
  const yarrr::Coordinate velocity_in_metres{ position_in_metres };
  yarrr::PhysicalParameters parameters;
  std::string id;
  std::unique_ptr< sol::state > lua;
  sol::table objects;
  yarrr::Object::Pointer object;
  std::unique_ptr< yarrr::ObjectModel > model;
};

