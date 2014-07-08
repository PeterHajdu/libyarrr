#pragma once

#include <yarrr/vector.hpp>
#include <cstdint>
#include <vector>

namespace yarrr
{
  typedef std::vector<char> Data;

  typedef Vector<int64_t> Coordinate;
  typedef Vector<int64_t> Velocity;
  typedef int16_t Angle;

  //one huplon is the quarter of one metre
  inline Coordinate& metres_to_huplons_in_place( Coordinate& coordinate_in_metres )
  {
    return coordinate_in_metres<<=2;
  }

  inline Coordinate metres_to_huplons( Coordinate coordinate_in_metres )
  {
    return metres_to_huplons_in_place( coordinate_in_metres );
  }

  inline Coordinate& huplons_to_metres_in_place( Coordinate& coordinate_in_huplons )
  {
    return coordinate_in_huplons>>=2;
  }

  inline Coordinate huplons_to_metres( Coordinate coordinate_in_huplons )
  {
    return huplons_to_metres_in_place( coordinate_in_huplons );
  }

}

