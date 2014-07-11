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
ShipControl::ShipControl( PhysicalParameters& object )
  : m_object( object )
{
}

void
ShipControl::handle_command( const Command& command )
{
  yarrr::travel_in_time_to( command.timestamp(), m_object );
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
    static_cast< int64_t >( back_engine_power * cos( m_object.angle * 3.14 / 180.0 / 4.0 ) ),
    static_cast< int64_t >( back_engine_power * sin( m_object.angle * 3.14 / 180.0 / 4.0 ) ) };
  m_object.velocity += heading;
}

void
ShipControl::spin( int power )
{
  m_object.vangle += power;
}

}

