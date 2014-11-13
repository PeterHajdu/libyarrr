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
    lua.reset( new the::model::Lua() );

    const yarrr::ObjectContainer& const_container( *container );
    exporter.reset( new yarrr::ObjectExporter( const_container, *lua ) );
  }

  void TearDown()
  {
    exporter.reset();
  }

  It( exposes_objects_table_to_the_lua_world )
  {
    AssertThat( lua->assert_that( "objects" ), Equals( true ) );
  }

  void assert_every_object_is_exported()
  {
    for ( const auto& id : object_ids )
    {
      const std::string object_path_exists( "objects[ " + the::model::to_lua_string( std::to_string( id ) ) + " ]" );
      AssertThat( lua->assert_that( object_path_exists ), Equals( true ) );
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
    const std::string removed_object_id( std::to_string( object_ids.back() ) );
    container->delete_object(  object_ids.back() );
    exporter->refresh();

    const std::string object_path_exists( "objects[ " + the::model::to_lua_string( removed_object_id ) + " ]" );
    AssertThat(  lua->assert_that( object_path_exists ), Equals( false ) );
  }

  const size_t number_of_objects{ 3 };
  std::unique_ptr< yarrr::ObjectContainer > container;
  std::unique_ptr< yarrr::ObjectExporter > exporter;
  std::unique_ptr< the::model::Lua > lua;
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

    lua.reset( new the::model::Lua() );
    objects.reset( new the::model::OwningNode( "objects", *lua ) );
    model.reset( new yarrr::ObjectModel( *object, *objects ) );

    object_path = std::string( "objects[" ) + the::model::to_lua_string( std::to_string( object->id() ) ) + "]";
  }

  void TearDown()
  {
    model.reset();
    objects.reset();
    lua.reset();
  }

  It( exports_the_objects_coordinate_from_physical_parameters )
  {
    AssertThat( lua->assert_equals( the::model::path_from( { object_path, "coordinate", "x" } ), position_in_metres.x ), Equals( true ) );
    AssertThat( lua->assert_equals( the::model::path_from( { object_path, "coordinate", "y" } ), position_in_metres.y ), Equals( true ) );
  }

  It( exports_the_objects_velocity_from_physical_parameters )
  {
    AssertThat( lua->assert_equals( the::model::path_from( { object_path, "velocity", "x" } ), velocity_in_metres.x ), Equals( true ) );
    AssertThat( lua->assert_equals( the::model::path_from( { object_path, "velocity", "y" } ), velocity_in_metres.y ), Equals( true ) );
  }

  It( exports_the_objects_orientation )
  {
    AssertThat( lua->assert_equals( the::model::path_from( { object_path, "orientation" } ), orientation_in_degrees ), Equals( true ) );
  }

  It( exports_the_objects_angular_velocity )
  {
    AssertThat( lua->assert_equals( the::model::path_from( { object_path, "angular_velocity" } ), angular_velocity_in_degrees ), Equals( true ) );
  }

  const yarrr::Angle orientation{ 1823839 };
  const double orientation_in_degrees{ yarrr::hiplon_to_degrees( orientation ) };

  const yarrr::Angle angular_velocity{ 18378239 };
  const double angular_velocity_in_degrees{ yarrr::hiplon_to_degrees( angular_velocity ) };

  const yarrr::Coordinate position{ 129833, -20000 };
  const yarrr::Coordinate position_in_metres{ yarrr::huplons_to_metres( position ) };
  const yarrr::Coordinate velocity_in_metres{ position_in_metres };
  yarrr::PhysicalParameters parameters;

  yarrr::Object::Pointer object;
  std::string object_path;

  std::unique_ptr< the::model::Lua > lua;
  std::unique_ptr< the::model::OwningNode > objects;
  std::unique_ptr< yarrr::ObjectModel > model;

};

