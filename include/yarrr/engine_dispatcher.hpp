#pragma once
#include <thectci/id.hpp>
#include <thectci/dispatcher.hpp>
#include <thectci/service_registry.hpp>

namespace yarrr
{

class EngineDispatcher : public the::ctci::Dispatcher
{
  public:
    add_ctci( "yarrr_engine_dispatcher" );
};

template < typename Event >
void engine_dispatch( const Event& event )
{
  the::ctci::service< EngineDispatcher >().dispatch( event );
}

}

