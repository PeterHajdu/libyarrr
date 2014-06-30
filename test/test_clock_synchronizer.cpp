#include <yarrr/clock_synchronizer.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{
  class TestClock
  {
    public:
      mutable uint64_t last_time_returned{ 0 };
      uint64_t last_set_time{ 0 };

      uint64_t now() const
      {
        last_time_returned = last_set_time;
        return last_set_time;
      }

      void set_time( uint64_t time )
      {
        last_set_time = time;
      }

      int64_t last_set_offset{ 0 };
      void set_offset( int64_t offset )
      {
        last_set_offset = offset;
      }
  };

  class TestConnection
  {
      static const size_t length_of_type{ 1 };
    public:
      bool message_was_sent{ false };
      uint64_t time_sent_in_last_message{ 0 };
      void send_from_network_thread( the::net::Data&& message )
      {
        message_was_sent = true;
        time_sent_in_last_message = extract_timestamp_from( message );
      }

      uint64_t extract_timestamp_from( const the::net::Data& message )
      {
        assert( message.size() >= length_of_type + sizeof( uint64_t ) );
        const uint64_t* timestamp( reinterpret_cast< const uint64_t* >( &message[ length_of_type ] ) );
        return *timestamp;
      }
  };
}

Describe(a_client_clock_synchronizer)
{
  void SetUp()
  {
    test_clock.reset( new TestClock() );
    test_clock->set_time( answer_arrive_time_client_side );
    test_connection.reset( new TestConnection() );
    test_synchronizer.reset( new TestSynchronizer( *test_clock, *test_connection ) );
  }

  void initiate()
  {
    test_synchronizer->wake_up();
    AssertThat( test_connection->message_was_sent, Equals( true ) );
  }

  It( initiates_clock_synchronization_on_the_first_wake_up )
  {
    initiate();
  }

  It( sends_the_current_time_when_initiated )
  {
    initiate();
    AssertThat( test_connection->time_sent_in_last_message, Equals( test_clock->last_time_returned ) );
  }

  the::net::Data create_answer_message( uint64_t init_time, uint64_t response_time )
  {
    const char* init_time_on_client_side_pointer( reinterpret_cast< const char * >( &init_time ) );
    const char* response_time_on_server_side_pointer( reinterpret_cast< const char * >( &response_time ) );
    the::net::Data response( 1, 1 );
    response.insert( end( response ),
        init_time_on_client_side_pointer, init_time_on_client_side_pointer + sizeof( init_time ) );
    response.insert( end( response ),
        response_time_on_server_side_pointer, response_time_on_server_side_pointer + sizeof( response_time ) );
    return response;
  }

  It( calculates_network_latency_from_the_answer_message )
  {
    test_synchronizer->on_message_from_network(
        create_answer_message( init_time_on_client_side, response_time_on_server_side ) );

    const uint64_t expected_network_latency( ( answer_arrive_time_client_side - init_time_on_client_side ) / 2 );
    AssertThat( test_synchronizer->network_latency(), Equals( expected_network_latency ) );
  }

  It( calculates_server_client_clock_offset_from_the_answer_message )
  {
    test_synchronizer->on_message_from_network(
        create_answer_message( init_time_on_client_side, response_time_on_server_side ) );

    const uint64_t network_latency( ( answer_arrive_time_client_side - init_time_on_client_side ) / 2 );
    const uint64_t expected_offset( response_time_on_server_side - network_latency - init_time_on_client_side );
    AssertThat( test_synchronizer->clock_offset(), Equals( expected_offset ) );
  }

  It( sets_clock_offset )
  {
    test_synchronizer->on_message_from_network( create_answer_message( init_time_on_client_side, response_time_on_server_side ) );
    test_synchronizer->synchronize_local_clock();
    AssertThat( test_clock->last_set_offset, Equals( test_synchronizer->clock_offset() ) );
  }

  const uint64_t init_time_on_client_side{ 1000 };
  const uint64_t response_time_on_server_side{ 200000 };
  const uint64_t answer_arrive_time_client_side{ 1500 };
  std::unique_ptr< TestClock > test_clock;
  std::unique_ptr< TestConnection > test_connection;
  typedef yarrr::clock_sync::Client< TestClock, TestConnection > TestSynchronizer;
  std::unique_ptr< TestSynchronizer > test_synchronizer;
};

