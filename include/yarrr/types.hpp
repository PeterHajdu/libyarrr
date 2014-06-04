#pragma once

#include <cstdint>

namespace yarrr
{
  struct Vector
  {
    typedef int64_t type;
    type x;
    type y;
  };

  inline const Vector& operator+=( Vector& l, const Vector& r )
  {
    l.x += r.x;
    l.y += r.y;
    return l;
  }

  inline bool operator==( const Vector& l, const Vector& r )
  {
    return
      l.x == r.x &&
      l.y == r.y;
  }
}

