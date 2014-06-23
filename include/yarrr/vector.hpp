#pragma once
#include <ostream>

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
  std::ostream& operator<<( std::ostream& output, const Vector<T>& r )
  {
    output << "( " << r.x << ", " << r.y << " )";
    return output;
  }

  template <typename T, typename M>
  inline Vector<T>& operator*=( Vector<T>& l, const M& multiplier )
  {
    l.x*=multiplier;
    l.y*=multiplier;
    return l;
  }

  template <typename T, typename M>
  inline Vector<T> operator*( Vector<T> l, const M& multiplier )
  {
    return l*=multiplier;
  }

  template <typename T>
  inline Vector<T>& operator+=( Vector<T>& l, const Vector<T>& r )
  {
    l.x += r.x;
    l.y += r.y;
    return l;
  }

  template <typename T>
  inline Vector<T> operator+( Vector<T> l, const Vector<T>& r )
  {
    return l+=r;
  }

  template <typename T>
  inline Vector<T>& operator-=( Vector<T>& l, const Vector<T>& r )
  {
    l.x -= r.x;
    l.y -= r.y;
    return l;
  }

  template <typename T>
  inline Vector<T> operator-( Vector<T> l, const Vector<T>& r )
  {
    return l-=r;
  }


  template <typename T>
  inline bool operator==( const Vector<T>& l, const Vector<T>& r )
  {
    return
      l.x == r.x &&
      l.y == r.y;
  }


  template <typename T>
  inline bool operator!=( const Vector<T>& l, const Vector<T>& r )
  {
    return !( l == r );
  }

  template <typename T>
  bool operator<( const Vector<T>& l, const Vector<T>& r )
  {
    return l.x * l.x + l.y * l.y < r.x * r.x + l.y * l.y;
  }

  template <typename T>
  bool operator>( const Vector<T>& l, const Vector<T>& r )
  {
    return r < l;
  }
}

