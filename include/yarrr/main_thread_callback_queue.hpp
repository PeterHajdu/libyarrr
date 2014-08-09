#pragma once

#include <yarrr/callback_queue.hpp>
#include <thectci/id.hpp>

namespace yarrr
{

class MainThreadCallbackQueue : public yarrr::CallbackQueue
{
  public:
    add_ctci( "yarrr_main_thread_callback_queue" );
};

}

