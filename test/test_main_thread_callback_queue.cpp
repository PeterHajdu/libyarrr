#include <yarrr/main_thread_callback_queue.hpp>
#include <yarrr/callback_queue.hpp>
#include <thectci/service_registry.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( a_main_thread_callback_queue )
{

  It( is_a_callback_queue )
  {
    yarrr::CallbackQueue& callback_queue(
        the::ctci::service< yarrr::MainThreadCallbackQueue >() );
    (void)callback_queue;
  }

  It( is_a_service )
  {
    yarrr::MainThreadCallbackQueue& callback_queue(
        the::ctci::service< yarrr::MainThreadCallbackQueue >() );
    (void)callback_queue;
  }

};

