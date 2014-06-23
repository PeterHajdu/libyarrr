#include <igloo/igloo_alt.h>
#include <yarrr/vector.hpp>

#include <sstream>

using namespace igloo;

typedef yarrr::Vector<int> Vector;

Describe(a_vector)
{

  It(can_be_pushed_to_a_stream)
  {
    std::stringstream stream;
    stream << a << b;
  }

  It(can_be_subtracted)
  {
    const Vector expected{ b.x - a.x, b.y - a.y };
    AssertThat( b - a, Equals( expected ) );
  }

  It(can_be_added)
  {
    const Vector expected{ b.x + a.x, b.y + a.y };
    AssertThat( b + a, Equals( expected ) );
  }

  It(can_be_compared_with_less_operator)
  {
    AssertThat( a < b, Equals( true ) );
    AssertThat( b < a, Equals( false ) );
    AssertThat( a < a, Equals( false ) );
  }

  It(can_be_compared_with_more_operator)
  {
    AssertThat( a > b, Equals( false ) );
    AssertThat( b > a, Equals( true ) );
    AssertThat( a > a, Equals( false ) );
  }

  It(can_be_compared_with_equal_operator )
  {
    AssertThat( a == b, Equals( false ) );
    AssertThat( a == a, Equals( true ) );
  }

  It(can_be_compared_with_not_equal_operator )
  {
    AssertThat( a != b, Equals( true ) );
    AssertThat( a != a, Equals( false ) );
  }

  It(can_be_multiplied)
  {
    AssertThat( a * 1, Equals( a ) );
    AssertThat( a * 2, Equals( b ) );
    AssertThat( a * 1.0, Equals( a ) );
    AssertThat( a * 2.0, Equals( b ) );
  }

  const Vector a{ 100, 10 };
  const Vector b{ 200, 20 };
};

