#include <yarrr/object_exporter.hpp>
#include <yarrr/object_container.hpp>
#include <yarrr/object.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/log.hpp>

namespace yarrr
{

ObjectExporter::ObjectExporter(
    const ObjectContainer& container,
    the::model::Lua& lua )
  : m_container( container )
  , m_objects_model( "objects", lua )
{
  refresh();
}

void
ObjectExporter::refresh()
{
  m_objects_model.clear();
  for ( const auto& object : m_container.objects() )
  {
    m_objects_model.add_node( the::model::Node::Pointer( new ObjectModel( *object.second, m_objects_model ) ) );
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

ObjectModel::ObjectModel( const Object& object, the::model::Node& parent )
  : Node( std::to_string( object.id() ), parent )
  , m_coordinate( "coordinate", extract_coordinate_from( object ), *this )
  , m_velocity( "velocity", extract_velocity_from( object ), *this )
  , m_orientation( "orientation", extract_orientation_from( object ), *this )
  , m_angular_velocity( "angular_velocity", extract_angular_velocity_from( object ), *this )
{
  thelog( log::debug )( "Exporting object model with id,", object.id() );
}

}

