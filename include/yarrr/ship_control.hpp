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

  private:

    void thruster();
    void spin( int power );

    Object& m_object;
};

}

