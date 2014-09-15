#include <yarrr/ship_control.hpp>
#include <yarrr/types.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/command.hpp>
#include <cmath>
#include <cstdint>

namespace
{
  const int64_t back_engine_power{ 5_metres };
  const int64_t cw_engine_power{ 8_degrees };
}

namespace yarrr
{
ShipControl::ShipControl( PhysicalParameters& physical_parameters )
  : m_physical_parameters( physical_parameters )
{
}

void
ShipControl::handle_command( const Command& command )
{
  yarrr::travel_in_time_to( command.timestamp(), m_physical_parameters );
  switch( command.type() )
  {
    case Command::main_thruster :
      thruster();
      break;
    case Command::port_thruster :
      spin( cw_engine_power );
      break;
    case Command::starboard_thruster :
      spin( -cw_engine_power );
      break;
  }
}

void
ShipControl::thruster()
{
  m_physical_parameters.velocity += heading( m_physical_parameters, back_engine_power );
}

void
ShipControl::spin( int power )
{
  m_physical_parameters.angular_velocity += power;
}

}

