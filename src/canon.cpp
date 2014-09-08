#include <yarrr/canon.hpp>
#include <yarrr/command.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/inventory.hpp>
#include <yarrr/object_creator.hpp>
#include <thectci/service_registry.hpp>
#include <yarrr/engine_dispatcher.hpp>
#include <iostream>

namespace yarrr
{

Canon::Canon()
: ObjectBehavior( do_not_syncronize )
, m_physical_parameters( nullptr )
, m_index( 0 )
, m_number_of_canons( 1 )
{
}


void
Canon::do_register_to( Object& owner )
{
  owner.dispatcher.register_listener< yarrr::Command  >(
      std::bind( &Canon::handle_command, this, std::placeholders::_1 ) );
  m_physical_parameters = &owner.components.component< yarrr::PhysicalBehavior >().physical_parameters;
  assert( owner.components.has_component< Inventory >() );
  owner.components.component< Inventory >().register_item( *this );

  if ( owner.components.has_component< Canon >() )
  {
    m_index = owner.components.component< Canon >().generate_next_index();
  }
}


int
Canon::generate_next_index()
{
  return m_number_of_canons++;
}


void
Canon::handle_command( const Command& command ) const
{
  if ( command.type() != Command::fire )
  {
    return;
  }

  assert( m_physical_parameters );

  the::ctci::service< yarrr::EngineDispatcher >().dispatch( ObjectCreated(
        create_laser( generate_physical_parameters() ) ) );
}


PhysicalParameters
Canon::generate_physical_parameters() const
{
  PhysicalParameters new_parameters( *m_physical_parameters );
  const yarrr::Coordinate difference( perpendicular( heading( new_parameters, 50 ) ) );
  const int left_or_right( m_index % 2 ? -1 : 1 );
  new_parameters.coordinate += difference * m_index * left_or_right * 0.5;
  return new_parameters;
}


ObjectBehavior::Pointer
Canon::clone() const
{
  return Pointer( new Canon() );
}

}

