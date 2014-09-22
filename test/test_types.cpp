#include <yarrr/types.hpp>
#include <igloo/igloo_alt.h>

#include <sstream>

using namespace igloo;

Describe(a_coordinate)
{
  void SetUp()
  {
    test_coordinate = { 512, 512 };
  }

  It(can_be_converted_to_huplons)
  {
    AssertThat( metres_to_huplons( test_coordinate ), Equals( yarrr::Coordinate{ 512 * 256, 512 * 256 } ) );
  }

  It(can_be_converted_to_metres)
  {
    AssertThat( huplons_to_metres( test_coordinate ), Equals( yarrr::Coordinate{ 2, 2 } ) );
  }

  yarrr::Coordinate test_coordinate;
};

Describe( converters )
{

  Describe( distance_conversion )
  {
    It( can_be_converted_to_metres )
    {
      yarrr::Coordinate::type length( 256000 );
      AssertThat( yarrr::huplons_to_metres( length ), Equals( 1000 ) );
    }
  };

  Describe( distance_literals )
  {

    It( converts_metres_to_huplons )
    {
      AssertThat( 10_metres, Equals( 2560 ) );
    }

  };

  Describe( angle_literals )
  {

    It( converts_to_and_from_degrees )
    {
      AssertThat( 10_degrees, Equals( 40 ) );
      AssertThat( yarrr::hiplon_to_degrees( 40 ), Equals( 10 ) );
    }


    It( converts_to_radians )
    {
      AssertThat( yarrr::hiplon_to_radians( 40 ), EqualsWithDelta( 0.174532925, 0.0001 ) );
    }

    It( converts_from_radians )
    {
      AssertThat( yarrr::radian_to_hiplons( yarrr::pi() ), EqualsWithDelta( 180_degrees, 1 ) );
    }

  };

};

