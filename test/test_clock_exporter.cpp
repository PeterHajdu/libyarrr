#include <yarrr/clock_exporter.hpp>
#include <yarrr/lua_engine.hpp>
#include <thetime/clock.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_clock_exporter)
{
  void SetUp()
  {
    lua.reset( new the::model::Lua() );
    exporter.reset( new yarrr::ClockExporter( clock, *lua ) );
  }

  void TearDown()
  {
    exporter.reset();
  }

  It( exposes_universe_time_function_to_the_lua_world )
  {
    AssertThat( lua->assert_that( "universe_time" ), Equals( true ) );
  }

  It_Only( returns_the_epoch_of_the_clock )
  {
    const auto now( std::to_string( clock.now() / the::time::Clock::ticks_per_second ) );
    const std::string time_diff_statement( "universe_time() - " + now );
    AssertThat( lua->assert_that( time_diff_statement + " >= 0" ), Equals( true ) );
    AssertThat( lua->assert_that( time_diff_statement + " < 2" ), Equals( true ) );
  }

  std::unique_ptr< the::model::Lua > lua;
  std::unique_ptr< yarrr::ClockExporter > exporter;
  the::time::Clock clock;
};

