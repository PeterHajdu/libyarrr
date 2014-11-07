#include <yarrr/lua_engine.hpp>
#include <igloo/igloo_alt.h>

#include <sol.hpp>

using namespace igloo;

Describe( a_lua_engine )
{
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

