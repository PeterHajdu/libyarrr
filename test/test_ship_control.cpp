#include <yarrr/ship_control.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/command.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(ship_control)
{
  void SetUp()
  {
    physical_parameters.timestamp = now;
    initiali_state = physical_parameters;
  }

  It( accepts_commands )
  {
    yarrr::Command test_command;
    test_control.handle_command( test_command );
  }

  It( can_accelerate_an_object )
  {
    test_control.handle_command( back_engine_command );
    AssertThat( physical_parameters.velocity, !Equals( initiali_state.velocity ) );
  }

  It( can_spin_an_object_cw )
  {
    test_control.handle_command( cw_command );
    AssertThat( physical_parameters.angular_velocity, IsGreaterThan( initiali_state.angular_velocity ) );
  }

  It( can_spin_an_object_ccw )
  {
    test_control.handle_command( ccw_command );
    AssertThat( physical_parameters.angular_velocity, IsLessThan( initiali_state.angular_velocity ) );
  }

  It( travels_object_in_time_to_apply_command )
  {
    test_control.handle_command( back_engine_command_in_future );
    AssertThat( physical_parameters.timestamp, IsGreaterThan( initiali_state.timestamp ) );
  }

  uint64_t now{ 0 };
  uint64_t future{ 100000 };
  yarrr::Command back_engine_command{ yarrr::Command::thruster, now };
  yarrr::Command back_engine_command_in_future{ yarrr::Command::thruster, future };
  yarrr::Command cw_command{ yarrr::Command::cw, now };
  yarrr::Command ccw_command{ yarrr::Command::ccw, now };
  yarrr::PhysicalParameters physical_parameters;
  yarrr::PhysicalParameters initiali_state;
  yarrr::ShipControl test_control{ physical_parameters };
};

