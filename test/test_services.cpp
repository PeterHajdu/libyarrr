#include <yarrr/engine_dispatcher.hpp>
#include <yarrr/main_thread_callback_queue.hpp>
#include <yarrr/test_graphical_engine.hpp>
#include <yarrr/object_factory.hpp>
#include <yarrr/mission_factory.hpp>
#include <thectci/service_registry.hpp>

#include "test_services.hpp"
#include "test_particle_factory.hpp"

namespace
{
  the::ctci::AutoServiceRegister< yarrr::MissionFactory, yarrr::MissionFactory >
    auto_mission_factory_register;

  the::ctci::AutoServiceRegister< yarrr::ObjectFactory, yarrr::ObjectFactory >
    auto_object_factory_register;

  the::ctci::AutoServiceRegister< yarrr::EngineDispatcher, yarrr::EngineDispatcher >
    auto_engine_dispatcher_register;

  the::ctci::AutoServiceRegister< yarrr::MainThreadCallbackQueue, yarrr::MainThreadCallbackQueue >
    auto_main_thread_callback_queue_register;

  the::ctci::AutoServiceRegister< yarrr::GraphicalEngine, test::GraphicalEngine >
    test_graphical_engine_register;

  the::ctci::AutoServiceRegister< yarrr::ParticleFactory, test::ParticleFactory >
    test_particle_factory_register;
}

void
test::clean_engine_dispatcher()
{
  the::ctci::service< yarrr::EngineDispatcher >().clear();
}

