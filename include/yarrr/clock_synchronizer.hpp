#pragma once

#include <thenet/network_task.hpp>
#include <atomic>

namespace yarrr
{
namespace clock_sync
{
  enum Id : char { protocol_id = 1 };

template < typename Clock, typename Connection >
class Server : public the::net::NetworkTask
{
  public:
    Server( Clock& clock, Connection& connection )
      : m_connection( connection )
      , m_clock( clock )
    {
    }

    virtual void wake_up() override
    {
    }

    virtual void on_message_from_network( const the::net::Data& message ) override
    {
      if ( is_malformed( message ) )
      {
        return;
      }

      the::net::Data response( begin( message ), begin( message ) + 9 );

      uint64_t timestamp( m_clock.now() );
      const char* timestamp_pointer( reinterpret_cast< const char * >( &timestamp ) );
      response.insert( end( response ), timestamp_pointer, timestamp_pointer + sizeof( timestamp ) );
      m_connection.send_on_network_thread( std::move( response ) );
    }

  private:
    bool is_malformed( const the::net::Data& message ) const
    {
      if ( message.size() != 9 )
      {
        return true;
      }

      return message[ 0 ] != protocol_id;
    }

    Connection& m_connection;
    Clock& m_clock;
};

template < typename Clock, typename Connection >
class Client : public the::net::NetworkTask
{
  public:
    Client( Clock& clock, Connection& connection )
      : m_connection( connection )
      , m_clock( clock )
      , m_latency( 0 )
      , m_offset( 0 )
      , m_last_initiated( 0 )
    {
    }

    virtual void wake_up() override
    {
      if ( m_last_initiated )
      {
        return;
      }

      const uint64_t timestamp( m_clock.now() );
      const char* timestamp_pointer( reinterpret_cast< const char * >( &timestamp ) );

      the::net::Data initiating_message( 1, protocol_id );
      initiating_message.insert(
          end( initiating_message ),
          timestamp_pointer, timestamp_pointer + sizeof( timestamp ) );
      m_connection.send_on_network_thread( std::move( initiating_message ) );
      m_last_initiated = timestamp;
    }

    virtual void on_message_from_network( const the::net::Data& message ) override
    {
      if ( is_malformed( message ) )
      {
        return;
      }

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
    bool is_malformed( const the::net::Data& message ) const
    {
      if ( message.size() != 17 )
      {
        return true;
      }

      return message[0] != protocol_id;
    }

    Connection& m_connection;
    Clock& m_clock;
    std::atomic<uint64_t> m_latency;
    std::atomic<int64_t> m_offset;
    uint64_t m_last_initiated;
};

}
}

