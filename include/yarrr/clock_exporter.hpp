#pragma once

#include <themodel/function.hpp>

namespace the
{

namespace time
{
class Clock;
}

}

namespace yarrr
{

class ClockExporter
{
  public:
    ClockExporter( const the::time::Clock&, the::model::Lua& );

  private:
    the::model::Function m_universe_time;
};

}

