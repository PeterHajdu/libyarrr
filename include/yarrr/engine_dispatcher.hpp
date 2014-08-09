#pragma once
#include <thectci/id.hpp>
#include <thectci/dispatcher.hpp>

namespace yarrr
{

class EngineDispatcher : public the::ctci::Dispatcher
{
  public:
    add_ctci( "yarrr_engine_dispatcher" );
};

}

