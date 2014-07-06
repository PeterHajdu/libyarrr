#pragma once

namespace yarrr
{
class Object;
class Command;

class ShipControl
{
  public:
    ShipControl( Object& object );
    void handle_command( const Command& );
};

}

