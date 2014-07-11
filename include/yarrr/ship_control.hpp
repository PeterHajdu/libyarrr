#pragma once

namespace yarrr
{
class PhysicalParameters;
class Command;

class ShipControl
{
  public:
    ShipControl( PhysicalParameters& object );
    void handle_command( const Command& );

  private:

    void thruster();
    void spin( int power );

    PhysicalParameters& m_object;
};

}

