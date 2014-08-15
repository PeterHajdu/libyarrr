#include <yarrr/engine.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <yarrr/ship_control.hpp>
#include <yarrr/command.hpp>
#include <yarrr/physical_parameters.hpp>

namespace yarrr
{

Engine::Engine()
  : ObjectBehavior( synchronize )
{
}

Engine::~Engine()
{
}

void
Engine::register_to( Object& owner )
{
  m_ship_control.reset(
      new ShipControl( owner.components.component< PhysicalBehavior >().physical_parameters ) );
  owner.dispatcher.register_listener< yarrr::Command  >(
      std::bind( &ShipControl::handle_command, *m_ship_control, std::placeholders::_1 ) );
  owner.components.register_component( *this );
}


ObjectBehavior::Pointer
Engine::clone() const
{
  return Pointer( new Engine() );
}

}

