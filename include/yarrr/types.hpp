#pragma once

namespace yarrr
{
  struct Vector
  {
    typedef int64_t type;
    type x;
    type y;
  };

  const Vector& operator+=( Vector& l, const Vector& r )
  {
    l.x += r.x;
    l.y += r.y;
    return l;
  }

  bool operator==( const Vector& l, const Vector& r )
  {
    return
      l.x == r.x &&
      l.y == r.y;
  }
}

