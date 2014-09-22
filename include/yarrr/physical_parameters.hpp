#pragma once
#include <yarrr/types.hpp>
#include <thetime/clock.hpp>
#include <string>

namespace yarrr
{

  class PhysicalParameters
  {
    public:
      Coordinate coordinate;
      Velocity velocity;
      Angle orientation;
      Angle angular_velocity;
      the::time::Clock::Time timestamp;
      int16_t integrity;
  };


  std::ostream& operator<<( std::ostream& output, const PhysicalParameters& );
  bool operator==( const PhysicalParameters& l, const PhysicalParameters& r );
  void travel_in_time_to( const the::time::Clock::Time& timestamp, PhysicalParameters& );
  Coordinate heading( const PhysicalParameters&, int multiplier );

  PhysicalParameters weight_arithmetic_mean(
      const PhysicalParameters&,
      const PhysicalParameters&,
      double ratio );
}

