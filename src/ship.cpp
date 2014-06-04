#include <yarrr/ship.hpp>
#include <sstream>

namespace yarrr
{
  Ship::Ship( const std::string& binary_data )
  {
    std::stringstream input( binary_data );
    std::string somestring;
    input >> somestring;
    input >> somestring;
    coordinate.x = std::stoll( somestring );
    input >> somestring;
    coordinate.y = std::stoll( somestring );
    input >> somestring;
    velocity.x = std::stoll( somestring );
    input >> somestring;
    velocity.y = std::stoll( somestring );
    input >> somestring;
    angle = std::stoll( somestring );
    input >> somestring;
    vangle = std::stoll( somestring );
  }

  const std::string serialize( const Ship& ship )
  {
    std::stringstream output;
    output << ship;
    return output.str();
  }

  Ship deserialize( const std::string& data )
  {
    return Ship( data );
  }

  std::ostream& operator<<( std::ostream& output, const Ship& ship )
  {
    output << "Ship "
      << ship.coordinate.x << " "
      << ship.coordinate.y << " "
      << ship.velocity.x << " "
      << ship.velocity.y << " "
      << ship.angle << " "
      << ship.vangle << " "
      << "\n";

    return output;
  }

  bool operator==( const Ship& l, const Ship& r )
  {
    return
      l.coordinate == r.coordinate &&
      l.velocity == r.velocity &&
      l.angle == r.angle &&
      l.vangle == r.vangle;
  }

}
