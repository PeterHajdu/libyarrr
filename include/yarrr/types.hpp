#pragma once

#include <cstdint>

namespace yarrr
{
  template <typename T>
  struct Vector
  {
    typedef T type;
    type x;
    type y;
  };

  template <typename T>
  inline const Vector<T>& operator+=( Vector<T>& l, const Vector<T>& r )
  {
    l.x += r.x;
    l.y += r.y;
    return l;
  }

  template <typename T>
  inline bool operator==( const Vector<T>& l, const Vector<T>& r )
  {
    return
      l.x == r.x &&
      l.y == r.y;
  }
}

