#pragma once
#include <yarrr/types.hpp>
#include <thetime/clock.hpp>
#include <string>

namespace yarrr
{

  class PhysicalParameters
  {
    public:
      typedef uint64_t Id;

      Id id;
      Coordinate coordinate;
      Velocity velocity;
      Angle angle;
      Angle vangle;
      the::time::Clock::Time timestamp;
  };


  std::ostream& operator<<( std::ostream& output, const PhysicalParameters& );
  bool operator==( const PhysicalParameters& l, const PhysicalParameters& r );
  void travel_in_time_to( const the::time::Clock::Time& timestamp, PhysicalParameters& );

}

