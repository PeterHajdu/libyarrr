#pragma once
#include <functional>
#include <thenet/ring_buffer.hpp>

namespace yarrr
{

class CallbackQueue
{
  public:
    typedef std::function<void()> Callback;
    void push_back( Callback callback )
    {
      m_callback_queue.push( callback );
    }

    void process_callbacks()
    {
      Callback callback;
      while ( m_callback_queue.pop( callback ) )
      {
        callback();
      }
    }

  private:
    the::net::RingBuffer< Callback, 100 > m_callback_queue;
};

}

