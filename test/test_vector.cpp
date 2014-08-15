#include <igloo/igloo_alt.h>
#include <yarrr/vector.hpp>

#include <sstream>

using namespace igloo;

typedef yarrr::Vector<int> Vector;

Describe(a_vector)
{
  It(has_a_constructor)
  {
    const Vector vector_with_constructor( 1, 2 );
    AssertThat( vector_with_constructor.x, Equals( 1 ) );
    AssertThat( vector_with_constructor.y, Equals( 2 ) );
  }

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

  It(can_shift_bits_in_both_directions)
  {
    const Vector c{ 1, 2 };
    AssertThat( c << 2, Equals( Vector{ 4, 8 } ) );

    const Vector d{ 16, 32 };
    AssertThat( d >> 2, Equals( Vector{ 4, 8 } ) );
  }

  It(has_perpendicular_operation)
  {
    AssertThat( yarrr::perpendicular( a ), Equals( Vector( 10, -100 ) ) );
  }

  It(has_length_operation)
  {
    AssertThat( yarrr::length_of( a ), Equals( 100 ) );
  }

  const Vector a{ 100, 10 };
  const Vector b{ 200, 20 };
};

