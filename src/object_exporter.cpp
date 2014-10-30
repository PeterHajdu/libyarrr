#include <yarrr/object_exporter.hpp>
#include <yarrr/object_container.hpp>
#include <yarrr/object.hpp>
#include <yarrr/basic_behaviors.hpp>

namespace yarrr
{

ObjectExporter::ObjectExporter( const ObjectContainer& container, sol::state& lua )
  : m_lua( lua )
  , m_container( container )
{
  reset_objects_table();
  refresh();
}

void
ObjectExporter::reset_objects_table()
{
  m_object_models.clear();
  m_objects = m_lua.create_table();
  m_lua[ "objects" ] = m_objects;
}

void
ObjectExporter::refresh()
{
  reset_objects_table();
  for ( const auto& object : m_container.objects() )
  {
    m_object_models.emplace_back( *object.second, m_lua );
    m_objects.set( std::to_string( object.second->id() ), m_object_models.back().table() );
  }
}

}

namespace
{
  yarrr::Coordinate extract_coordinate_from( const yarrr::Object& object )
  {
    return yarrr::huplons_to_metres( yarrr::component_of< yarrr::PhysicalBehavior >( object ).physical_parameters.coordinate );
  }

  yarrr::Coordinate extract_velocity_from( const yarrr::Object& object )
  {
    return yarrr::huplons_to_metres( yarrr::component_of< yarrr::PhysicalBehavior >( object ).physical_parameters.velocity );
  }

  double extract_orientation_from( const yarrr::Object& object )
  {
    return yarrr::hiplon_to_degrees( yarrr::component_of< yarrr::PhysicalBehavior >( object ).physical_parameters.orientation );
  }

  double extract_angular_velocity_from( const yarrr::Object& object )
  {
    return yarrr::hiplon_to_degrees( yarrr::component_of< yarrr::PhysicalBehavior >( object ).physical_parameters.angular_velocity );
  }
}

namespace yarrr
{

ObjectModel::ObjectModel( const Object& object, sol::state& lua )
  : m_object_table( lua.create_table() )
  , m_coordinate( extract_coordinate_from( object ) )
  , m_coordinate_table( lua.create_table() )
  , m_velocity( extract_velocity_from( object ) )
  , m_velocity_table( lua.create_table() )
  , m_orientation( extract_orientation_from( object ) )
  , m_angular_velocity( extract_angular_velocity_from( object ) )
{
  thelog( log::debug )( "Exporting coordinate of object,", object.id(), m_coordinate );
  m_object_table[ "coordinate" ] = m_coordinate_table;
  m_coordinate_table[ "x" ] = m_coordinate.x;
  m_coordinate_table[ "y" ] = m_coordinate.y;

  m_object_table[ "velocity" ] = m_velocity_table;
  m_velocity_table[ "x" ] = m_velocity.x;
  m_velocity_table[ "y" ] = m_velocity.y;

  m_object_table[ "orientation" ] = m_orientation;
  m_object_table[ "angular_velocity" ] = m_angular_velocity;
}

sol::table&
ObjectModel::table()
{
  return m_object_table;
}

}

