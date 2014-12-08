#pragma once

#include <thetime/clock.hpp>

namespace yarrr
{

class TimerUpdate
{
  public:
    add_ctci( "yarrr_timer_update" );
    TimerUpdate( const the::time::Time& timestamp )
      : timestamp( timestamp )
    {
    }

    const the::time::Time& timestamp;
};


}

