#include <yarrr/lua_setup.hpp>
#include <yarrr/lua_engine.hpp>
#include <theconf/configuration.hpp>
#include <yarrr/log.hpp>

namespace
{

void load_configuration()
{
  const std::string config_file_path{
    the::conf::get< std::string >(  "lua_configuration_path" ) +
    "config.lua" };

  thelog( yarrr::log::info )( "Loading lua configuration from:", config_file_path );

  try
  {
    yarrr::LuaEngine::state().open_file( config_file_path );
  }
  catch( std::exception& e )
  {
    thelog( yarrr::log::info )( "Failed to load lua configuration:", e.what() );
    throw e;
  }
  thelog( yarrr::log::info )( "Finished loading lua configuration." );
}

void export_yarrr_stuff();

}

namespace yarrr
{

void initialize_lua_engine()
{
  export_yarrr_stuff();
  load_configuration();
}

}

//todo: export every type at it's definition when moved to the library
#include <yarrr/types.hpp>
#include <yarrr/shape.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/inventory.hpp>
#include <yarrr/collider.hpp>
#include <yarrr/destruction_handlers.hpp>
#include <yarrr/shape_behavior.hpp>
#include <yarrr/thruster.hpp>
#include <yarrr/canon.hpp>
#include <yarrr/mission.hpp>
#include <yarrr/object_decorator.hpp>
#include <yarrr/object_identity.hpp>
#include <yarrr/lua_agent.hpp>
namespace
{

void
export_yarrr_stuff()
{
  sol::state& lua( yarrr::LuaEngine::state() );
  lua[ "lua_configuration_path" ] = the::conf::get< std::string >( "lua_configuration_path" );
  lua.new_userdata< yarrr::Coordinate, int, int >( "Coordinate" );
  lua.new_userdata< yarrr::Tile::Coordinate, int, int >( "TileCoordinate" );
  lua.new_userdata< yarrr::Tile, yarrr::Tile::Coordinate, yarrr::Tile::Coordinate >( "Tile" );
  lua.new_userdata< yarrr::PhysicalBehavior >( "PhysicalBehavior" );
  lua.new_userdata< yarrr::ObjectIdentity, std::string >( "ObjectIdentity" );

  lua.new_userdata< yarrr::ObjectDecorator, yarrr::Object& >( "Object",
      "add_behavior", &yarrr::ObjectDecorator::add_behavior_clone,
      "id", &yarrr::ObjectDecorator::id,
      "move_to", &yarrr::ObjectDecorator::move_to,
      "set_velocity", &yarrr::ObjectDecorator::set_velocity,
      "rotates_with", &yarrr::ObjectDecorator::rotates_with,
      "fire", &yarrr::ObjectDecorator::fire,
      "destroy_self", &yarrr::ObjectDecorator::destroy_self );

  lua.new_userdata< yarrr::Inventory >( "Inventory" );
  lua.new_userdata< yarrr::Collider, int >( "Collider" );
  lua[ "ship_layer" ] = int( yarrr::Collider::ship_layer );
  lua.new_userdata< yarrr::DamageCauser, int >( "DamageCauser" );
  lua.new_userdata< yarrr::LootDropper >( "LootDropper" );
  lua.new_userdata< yarrr::Shape >( "Shape", "add_tile", &yarrr::Shape::add_tile );
  lua.new_userdata< yarrr::ShapeBehavior, yarrr::Shape >( "ShapeBehavior", "shape", &yarrr::ShapeBehavior::shape );
  lua.new_userdata< yarrr::ShapeGraphics >( "ShapeGraphics" );

  lua[ "main_thruster" ] = int( yarrr::ShipControl::main_thruster );
  lua[ "port_thruster" ] = int( yarrr::ShipControl::port_thruster );
  lua[ "starboard_thruster" ] = int( yarrr::ShipControl::starboard_thruster );
  lua.new_userdata< yarrr::Thruster, int, yarrr::Tile::Coordinate, yarrr::Angle >( "Thruster" );
  lua.new_userdata< yarrr::Canon, yarrr::Tile::Coordinate, yarrr::Angle >( "Canon" );

  lua.set_function( "degrees", yarrr::degree_to_hiplons );
  lua.set_function( "metres", yarrr::metre_to_huplons );
  lua.new_userdata< yarrr::Mission::Info, std::string, std::string >( "MissionInfo" );
  lua.new_userdata< yarrr::Mission::Objective, std::string, sol::function >( "MissionObjective" );

  lua.new_userdata< yarrr::Mission, yarrr::Mission::Info >( "Mission",
      "add_objective", &yarrr::Mission::add_objective,
      "id", &yarrr::Mission::string_id );

  lua[ "ongoing" ] = int( yarrr::ongoing );
  lua[ "succeeded" ] = int( yarrr::succeeded );
  lua[ "failed" ] = int( yarrr::failed );

  lua.new_userdata< yarrr::LuaFunction, sol::function >( "LuaFunction" );
  lua.new_userdata< yarrr::CallWhenDestroyed, yarrr::LuaFunction >( "CallWhenDestroyed" );
  lua.new_userdata< yarrr::LuaAgent, yarrr::LuaFunction, long >( "LuaAgent" );
}

}

