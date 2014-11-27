#include <yarrr/lua_engine.hpp>
#include <igloo/igloo_alt.h>

#include <sol.hpp>

using namespace igloo;

Describe( a_lua_engine )
{
  It( can_run_a_script )
  {
    AssertThat( yarrr::LuaEngine::run( "called = true\n" ), Equals( true ) );
    AssertThat( yarrr::LuaEngine::model().assert_equals( "called", true ), Equals( true ) );
    AssertThat( yarrr::LuaEngine::run( "assert( false )\n" ), Equals( false ) );
  }

  It( is_a_singleton )
  {
    yarrr::LuaEngine& lua_engine( yarrr::LuaEngine::instance() );
    AssertThat( &yarrr::LuaEngine::instance(), Equals( &lua_engine ) );
  }

  It( can_return_the_sol_state )
  {
    sol::state& sol_state( yarrr::LuaEngine::state() );
    (void) sol_state;
  }

  It( loads_base_libraries )
  {
    try
    {
      yarrr::LuaEngine::state().script( "print( \"hello lua\" ) " );
      yarrr::LuaEngine::state().script( "print( math.sqrt( 25 ) ) " );
      yarrr::LuaEngine::state().script( "print( os.time() ) " );
    }
    catch ( std::exception& e )
    {
      std::cout << e.what() << std::endl;
      throw e;
    }
  }

  It( returns_the_lua_model_with_the_global_table )
  {
    the::model::Lua& model( yarrr::LuaEngine::model() );
    (void)model;
  }

};

Describe( an_auto_lua_register )
{
  It( calls_the_function_when_constructed_with_the_lua_state_as_parameter )
  {
    yarrr::LuaEngine* called_with{ nullptr };
    yarrr::AutoLuaRegister auto_lua_register(
        [ &called_with ]( yarrr::LuaEngine& lua )
        {
          called_with = &lua;
        } );
    AssertThat( called_with, Equals( &yarrr::LuaEngine::instance() ) );
  }
};

namespace
{

class class_without_constructor_parameter_and_function
{
};

class class_with_constructor_parameter_and_without_functions
{
  public:
    class_with_constructor_parameter_and_without_functions( int a )
    {
    }
};

}

Describe( a_simple_lua_register )
{
  It( registers_a_type_without_constructor_parameters_and_exported_functions )
  {
    yarrr::SimpleLuaRegister< class_without_constructor_parameter_and_function >( "a_class" );
    yarrr::LuaEngine::state().script( "a_class.new()" );
  }

  It( registers_a_type_with_constructor_parameters_and_without_exported_functions )
  {
    yarrr::SimpleLuaRegister< class_with_constructor_parameter_and_without_functions, int >( "another_class" );
    yarrr::LuaEngine::state().script( "another_class.new( 10 )" );
  }
};

Describe_Only( a_lua_function_wrapper )
{

  void set_function( const std::string& name )
  {
    sol::function sol_function( yarrr::LuaEngine::state().get< sol::function >( name ) );
    function = std::make_unique< yarrr::LuaFunction >( sol_function );
  }

  void SetUp()
  {
    lua = &yarrr::LuaEngine::model();
    AssertThat( yarrr::LuaEngine::run(
          "was_test_function_called = false\n"
          "with_argument = 123\n"
          "function test_function( argument )\n"
          "was_test_function_called = true\n"
          "with_argument = argument\n"
          "end\n"
          "function exception_thrower()\n"
          "assert(false)\n"
          "end\n"),
        Equals( true ) );
    set_function( "test_function" );
  }

  It( can_be_called )
  {
    AssertThat( function->call( 123 ), Equals( true ) );
    AssertThat(
        lua->assert_equals( "was_test_function_called", true ),
        Equals( true ) );
  }

  It( forwards_arguments )
  {
    const int argument{ 987 };
    AssertThat( function->call( argument ), Equals( true ) );
    AssertThat(
        lua->assert_equals( "with_argument", argument ),
        Equals( true ) );
  }

  It( catches_exceptions )
  {
    set_function( "exception_thrower" );
    AssertThat( function->call(), Equals( false ) );
  }

  std::unique_ptr< yarrr::LuaFunction > function;
  the::model::Lua* lua;
};

