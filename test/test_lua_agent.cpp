#include <yarrr/lua_agent.hpp>
#include <yarrr/lua_engine.hpp>
#include <yarrr/timer_update.hpp>
#include <thectci/dispatcher.hpp>
#include <thectci/component_registry.hpp>
#include <themodel/lua.hpp>
#include "test_services.hpp"
#include "test_synchronizable_behavior.hpp"
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_lua_agent )
{

  void set_up_lua()
  {
    lua = std::make_unique< the::model::Lua >();
    AssertThat( lua->run(
          "number_of_function_calls = 0\n"
          "function the_function( object )\n"
          "number_of_function_calls = number_of_function_calls + 1\n"
          "end\n"
          ), Equals( true ) );
  }

  auto create_agent()
  {
    return std::make_unique< yarrr::LuaAgent >(
        yarrr::LuaFunction( lua->state()[ "the_function" ] ),
        period );
  }

  void update_at( const the::time::Time& timestamp )
  {
    object->dispatcher.dispatch( yarrr::TimerUpdate( timestamp ) );
  }

  void SetUp()
  {
    set_up_lua();
    object.reset( new yarrr::Object() );
    object->add_behavior( create_agent() );
    update_at( first_update_time );
  }

  It( calls_the_lua_function_on_the_first_timer_update )
  {
    AssertThat( lua->assert_equals( "number_of_function_calls", 1 ), Equals( true ) );
  }

  It( does_not_call_the_function_before_the_next_period )
  {
    update_at( before_next_period );
    AssertThat( lua->assert_equals( "number_of_function_calls", 1 ), Equals( true ) );
  }

  It( calls_the_function_in_the_next_period )
  {
    update_at( after_next_period );
    AssertThat( lua->assert_equals( "number_of_function_calls", 2 ), Equals( true ) );
  }

  yarrr::Object::Pointer object;
  std::unique_ptr< the::model::Lua > lua;

  const the::time::Time period{ 1000000u };
  const the::time::Time first_update_time{ 100u };
  const the::time::Time before_next_period{ first_update_time + period - 100 };
  const the::time::Time after_next_period{ first_update_time + period };

};

