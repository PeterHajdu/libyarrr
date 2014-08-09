#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/main_thread_callback_queue.hpp>
#include <thectci/service_registry.hpp>

#include "test_graphical_engine.hpp"

namespace
{
  the::ctci::AutoServiceRegister< yarrr::EngineDispatcher, yarrr::EngineDispatcher >
    auto_engine_dispatcher_register;

  the::ctci::AutoServiceRegister< yarrr::MainThreadCallbackQueue, yarrr::MainThreadCallbackQueue >
    auto_main_thread_callback_queue_register;

  the::ctci::AutoServiceRegister< yarrr::GraphicalEngine, test::GraphicalEngine >
    test_graphical_engine_register;
}

