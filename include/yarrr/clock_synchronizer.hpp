#pragma once

#include <thenet/network_task.hpp>
#include <atomic>

namespace yarrr
{
namespace clock_sync
{

template < typename Clock, typename Connection >
class Client : public the::net::NetworkTask
{
  public:
    Client( Clock& clock, Connection& connection )
      : m_connection( connection )
      , m_clock( clock )
      , m_latency( 0 )
      , m_offset( 0 )
    {
    }

    virtual void wake_up() override
    {
      const uint64_t timestamp( m_clock.now() );
      const char* timestamp_pointer( reinterpret_cast< const char * >( &timestamp ) );

      the::net::Data initiating_message( 1, 1 );
      initiating_message.insert(
          end( initiating_message ),
          timestamp_pointer, timestamp_pointer + sizeof( timestamp ) );
      m_connection.send_from_network_thread( std::move( initiating_message ) );
    }

    virtual void on_message_from_network( const the::net::Data& message ) override
    {
      const uint64_t initiation_time( *reinterpret_cast< const uint64_t* >( &message[ 1 ] ) );
      const uint64_t response_time( *reinterpret_cast< const uint64_t* >( &message[ 9 ] ) );
      const uint64_t response_arrive_time( m_clock.now() );
      m_latency.store( ( response_arrive_time - initiation_time ) / 2 );
      m_offset.store( ( response_time - m_latency - initiation_time ) );
    }

    uint64_t network_latency() const
    {
      return m_latency.load();
    }

    int64_t clock_offset() const
    {
      return m_offset.load();
    }

    void synchronize_local_clock()
    {
      m_clock.set_offset( clock_offset() );
    }

  private:
    Connection& m_connection;
    Clock& m_clock;
    std::atomic<uint64_t> m_latency;
    std::atomic<int64_t> m_offset;
};

}
}

