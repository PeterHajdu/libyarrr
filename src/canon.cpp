#include <yarrr/canon.hpp>
#include <yarrr/ship_control.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/inventory.hpp>
#include <yarrr/object_creator.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/lua_engine.hpp>
#include <yarrr/bitmagic.hpp>
#include <yarrr/object_created.hpp>
#include <yarrr/log.hpp>
#include <yarrr/coordinate_transformation.hpp>
#include <yarrr/object_identity.hpp>

#include <thectci/service_registry.hpp>

namespace
{
  const char * canon_name = "canon";
  yarrr::AutoEntityRegister< yarrr::Canon > auto_canon_register;
}

namespace yarrr
{

Canon::Canon()
: Item( rarely_synchronize(), canon_name, { 0, 0 } )
, m_physical_parameters( nullptr )
, m_orientation( 0 )
{
}

Canon::Canon( const Tile::Coordinate coordinate, const Angle& orientation )
: Item( rarely_synchronize(), canon_name, coordinate )
, m_physical_parameters( nullptr )
, m_orientation( orientation )
{
}

Canon::Canon( const Id& id, const Tile::Coordinate coordinate, const Angle& orientation )
: Item( rarely_synchronize(), id, canon_name, coordinate )
, m_physical_parameters( nullptr )
, m_orientation( orientation )
{
}

void
Canon::register_item_to( Object& owner )
{
  owner.dispatcher.register_listener< yarrr::ShipControl >(
      [ this ]( const ShipControl& control ){ handle_command( control ); } );
  m_physical_parameters = &owner.components.component< PhysicalBehavior >().physical_parameters;
}


void
Canon::handle_command( const ShipControl& command ) const
{
  if ( command.type() != ShipControl::fire )
  {
    return;
  }

  assert( m_physical_parameters );
  assert( has_component< ObjectIdentity >( *m_object ) );

  the::ctci::service< EngineDispatcher >().dispatch( ObjectCreated(
        create_laser(
          generate_physical_parameters(),
          component_of< ObjectIdentity >( *m_object )
          ) ) );
}


PhysicalParameters
Canon::generate_physical_parameters() const
{
  PhysicalParameters new_parameters( *m_physical_parameters );


  new_parameters.coordinate = transform_center_of_mass_relative_to_absolute(
      relative_coordinate(),
      m_physical_parameters->coordinate,
      m_physical_parameters->orientation );

  new_parameters.orientation += m_orientation;

  thelog( log::debug )( "shooting: ", new_parameters );

  return new_parameters;
}


ObjectBehavior::Pointer
Canon::clone() const
{
  return Pointer( new Canon( id(), tile_coordinate(), m_orientation ) );
}

Angle
Canon::orientation() const
{
  return m_orientation;
}

void
Canon::serialize_item( Serializer& serializer ) const
{
  serializer.push_back( m_orientation );
}

void
Canon::deserialize_item( Deserializer& deserializer )
{
  m_orientation = deserializer.pop_front< Angle >();
}

}

