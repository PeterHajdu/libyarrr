#include <yarrr/object.hpp>
#include <sstream>

namespace yarrr
{
  Object::Object( const std::string& binary_data )
  {
    std::stringstream input( binary_data );
    std::string somestring;
    input >> somestring;
    input >> somestring;
    id = std::stoll( somestring );
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
    input >> somestring;
    timestamp = std::stoull( somestring );
  }

  const std::string serialize( const Object& object )
  {
    std::stringstream output;
    output << object;
    return output.str();
  }

  Object deserialize( const std::string& data )
  {
    return Object( data );
  }

  std::ostream& operator<<( std::ostream& output, const Object& object )
  {
    output << "object "
      << object.id << " "
      << object.coordinate.x << " "
      << object.coordinate.y << " "
      << object.velocity.x << " "
      << object.velocity.y << " "
      << object.angle << " "
      << object.vangle << " "
      << object.timestamp << " "
      << "\n";

    return output;
  }

  bool operator==( const Object& l, const Object& r )
  {
    return
      l.coordinate == r.coordinate &&
      l.velocity == r.velocity &&
      l.angle == r.angle &&
      l.vangle == r.vangle;
  }

}
