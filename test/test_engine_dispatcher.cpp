#include <yarrr/engine_dispatcher.hpp>
#include <thectci/service_registry.hpp>
#include <thectci/dispatcher.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( an_engine_dispatcher )
{

  It( is_a_service_and_a_dispatcher )
  {
    the::ctci::Dispatcher& dispatcher( the::ctci::service< yarrr::EngineDispatcher >() );
    (void)dispatcher;
  }

};

