#include <yarrr/clock_exporter.hpp>
#include <themodel/lua.hpp>
#include <thetime/clock.hpp>

namespace yarrr
{

ClockExporter::ClockExporter( const the::time::Clock& clock, the::model::Lua& lua )
  : m_universe_time( "universe_time", lua,
      [ &clock ]()
      {
        return clock.now() / the::time::Clock::ticks_per_second;
      } )
{
}


}

