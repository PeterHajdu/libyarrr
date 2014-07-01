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
}

