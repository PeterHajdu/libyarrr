#include <yarrr/engine_dispatcher.hpp>
#include <thectci/service_registry.hpp>
#include <thectci/dispatcher.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{
  void ensure_linkage( const the::ctci::Dispatcher& dispatcher )
  {
    class some_class
    {
      public:
        add_ctci( "test_some_class" );
    };

    dispatcher.dispatch( some_class() );
  }

}

Describe( an_engine_dispatcher )
{

  It( is_a_dispatcher )
  {
    the::ctci::Dispatcher& dispatcher( the::ctci::service< yarrr::EngineDispatcher >() );
    ensure_linkage( dispatcher );
  }

  It( is_a_service )
  {
    yarrr::EngineDispatcher& engine_dispatcher( the::ctci::service< yarrr::EngineDispatcher >() );
    (void)engine_dispatcher;
  }

};

