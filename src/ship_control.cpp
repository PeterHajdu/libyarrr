#include <yarrr/ship_control.hpp>
#include <yarrr/types.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/command.hpp>
#include <cmath>
#include <cstdint>

namespace
{
  const float back_engine_power{ 40.0 };
  const int cw_engine_power{ 100 };
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
    case Command::thruster :
      thruster();
      break;
    case Command::cw :
      spin( cw_engine_power );
      break;
    case Command::ccw :
      spin( -cw_engine_power );
      break;
  }
}

void
ShipControl::thruster()
{
  const yarrr::Coordinate heading{
    static_cast< int64_t >( back_engine_power * cos( m_physical_parameters.angle * 3.14 / 180.0 / 4.0 ) ),
    static_cast< int64_t >( back_engine_power * sin( m_physical_parameters.angle * 3.14 / 180.0 / 4.0 ) ) };
  m_physical_parameters.velocity += heading;
}

void
ShipControl::spin( int power )
{
  m_physical_parameters.vangle += power;
}

}

