#include <yarrr/ship_control.hpp>
#include <yarrr/object.hpp>
#include <yarrr/command.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(ship_control)
{
  void SetUp()
  {
    object.timestamp = now;
    initiali_state = object;
  }

  It( accepts_commands )
  {
    yarrr::Command test_command;
    test_control.handle_command( test_command );
  }

  It( can_accelerate_an_object )
  {
    test_control.handle_command( back_engine_command );
    AssertThat( object.velocity, !Equals( initiali_state.velocity ) );
  }

  It( can_spin_an_object_cw )
  {
    test_control.handle_command( cw_command );
    AssertThat( object.vangle, IsGreaterThan( initiali_state.vangle ) );
  }

  It( can_spin_an_object_ccw )
  {
    test_control.handle_command( ccw_command );
    AssertThat( object.vangle, IsLessThan( initiali_state.vangle ) );
  }

  It( travels_object_in_time_to_apply_command )
  {
    test_control.handle_command( back_engine_command_in_future );
    AssertThat( object.timestamp, IsGreaterThan( initiali_state.timestamp ) );
  }

  uint64_t now{ 0 };
  uint64_t future{ 100000 };
  yarrr::Command back_engine_command{ yarrr::Command::thruster, now };
  yarrr::Command back_engine_command_in_future{ yarrr::Command::thruster, future };
  yarrr::Command cw_command{ yarrr::Command::cw, now };
  yarrr::Command ccw_command{ yarrr::Command::ccw, now };
  yarrr::Object object;
  yarrr::Object initiali_state;
  yarrr::ShipControl test_control{ object };
};

