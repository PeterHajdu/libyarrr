#pragma once

#include <thenet/network_task.hpp>
#include <thectci/id.hpp>
#include <yarrr/bitmagic.hpp>
#include <atomic>

namespace yarrr
{
namespace clock_sync
{
add_ctci( "clock_sync" );
enum : the::ctci::Id { protocol_id = ctci };

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

      //todo: use the same deserializer in is_malformed
      const uint64_t timestamp( m_clock.now() );
      yarrr::Deserializer request( message );
      request.pop_front<the::ctci::Id>();

      yarrr::Data response_buffer;
      yarrr::Serializer response( response_buffer );
      response.push_back( protocol_id );
      response.push_back( request.pop_front<uint64_t>() );
      response.push_back( timestamp );
      m_connection.send_on_network_thread( std::move( response_buffer ) );
    }

  private:
    bool is_malformed( const the::net::Data& message ) const
    {
      if ( message.size() != 12 )
      {
        return true;
      }

      const auto protocol_id_of_message( yarrr::Deserializer( message ).pop_front<the::ctci::Id>() );
      return protocol_id_of_message != protocol_id;
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
      m_last_initiated = m_clock.now();

      the::net::Data initiating_message;
      yarrr::Serializer serializer( initiating_message );
      serializer.push_back( protocol_id );
      serializer.push_back( m_last_initiated );
      m_connection.send_on_network_thread( std::move( initiating_message ) );
    }

    virtual void on_message_from_network( const the::net::Data& message ) override
    {
      if ( is_malformed( message ) )
      {
        return;
      }

      //todo: use the same deserializer in is_malformed
      const uint64_t response_arrive_time( m_clock.now() );

      yarrr::Deserializer deserializer( message );
      deserializer.pop_front< the::ctci::Id >();
      const auto initiation_time( deserializer.pop_front<uint64_t>() );
      const auto response_time( deserializer.pop_front<uint64_t>() );
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
      if ( message.size() != 20 )
      {
        return true;
      }

      const auto protocol_id_of_message( yarrr::Deserializer( message ).pop_front<the::ctci::Id>() );
      return protocol_id_of_message != protocol_id;
    }

    Connection& m_connection;
    Clock& m_clock;
    std::atomic<uint64_t> m_latency;
    std::atomic<int64_t> m_offset;
    uint64_t m_last_initiated;
};

}
}

