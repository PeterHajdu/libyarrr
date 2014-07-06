#include <yarrr/ship_control.hpp>
#include <yarrr/object.hpp>
#include <yarrr/command.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(ship_control)
{
  It( accepts_commands )
  {
    yarrr::Command test_command;
    test_control.handle_command( test_command );
  }


  yarrr::Object object;
  yarrr::ShipControl test_control{ object };
};

