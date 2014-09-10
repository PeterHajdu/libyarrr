#include <yarrr/types.hpp>
#include <igloo/igloo_alt.h>

#include <sstream>

using namespace igloo;

Describe(a_coordinate)
{
  void SetUp()
  {
    test_coordinate = { 100, 100 };
  }

  It(can_be_converted_to_huplons)
  {
    AssertThat( metres_to_huplons( test_coordinate ), Equals( yarrr::Coordinate{ 400, 400 } ) );
  }

  It(can_be_converted_to_metres)
  {
    AssertThat( huplons_to_metres( test_coordinate ), Equals( yarrr::Coordinate{ 25, 25 } ) );
  }

  yarrr::Coordinate test_coordinate;
};

Describe( converter_literals )
{

  Describe( distance_literals )
  {

    It( converts_metres_to_huplons )
    {
      AssertThat( 10_metres, Equals( 40 ) );
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

  };

};

