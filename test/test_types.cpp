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

