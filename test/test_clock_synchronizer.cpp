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
      static const size_t length_of_protocol_id{ sizeof( the::ctci::Id ) };
      static const size_t length_of_timestamp{ 8 };
    public:
      void reset()
      {
        message_was_sent = false;
        init_time_sent_in_last_message = 0;
        response_time_sent_in_last_message = 0;
      }

      bool message_was_sent{ false };
      uint64_t init_time_sent_in_last_message{ 0 };
      uint64_t response_time_sent_in_last_message{ 0 };
      void send_on_network_thread( the::net::Data&& message )
      {
        message_was_sent = true;
        extract_timestamps_from( message );
      }

      void extract_timestamps_from( yarrr::Deserializer message )
      {
        message.pop_front< the::ctci::Id >();
        init_time_sent_in_last_message = message.pop_front< uint64_t >();
        if ( message.bytes_left() < sizeof( uint64_t ) )
        {
          return;
        }

        response_time_sent_in_last_message = message.pop_front< uint64_t >();
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
  }

  It( initiates_clock_synchronization_only_on_the_first_wake_up )
  {
    initiate();
    AssertThat( test_connection->message_was_sent, Equals( true ) );
    test_connection->reset();
    initiate();
    AssertThat( test_connection->message_was_sent, Equals( false ) );
  }

  It( sends_the_current_time_when_initiated )
  {
    initiate();
    AssertThat( test_connection->init_time_sent_in_last_message, Equals( test_clock->last_time_returned ) );
  }

  the::net::Data create_answer_message()
  {
    the::net::Data response;
    yarrr::Serializer serializer( response );
    serializer.push_back( yarrr::clock_sync::protocol_id );
    serializer.push_back( init_time_on_client_side );
    serializer.push_back( response_time_on_server_side );
    return response;
  }

  It( calculates_network_latency_from_the_answer_message )
  {
    test_synchronizer->on_message_from_network( create_answer_message() );

    const uint64_t expected_network_latency( ( answer_arrive_time_client_side - init_time_on_client_side ) / 2 );
    AssertThat( test_synchronizer->network_latency(), Equals( expected_network_latency ) );
  }

  It( calculates_server_client_clock_offset_from_the_answer_message )
  {
    test_synchronizer->on_message_from_network( create_answer_message() );

    const uint64_t network_latency( ( answer_arrive_time_client_side - init_time_on_client_side ) / 2 );
    const int64_t expected_offset( response_time_on_server_side - network_latency - init_time_on_client_side );
    AssertThat( test_synchronizer->clock_offset(), Equals( expected_offset ) );
  }

  It( sets_clock_offset )
  {
    test_synchronizer->on_message_from_network( create_answer_message() );
    test_synchronizer->synchronize_local_clock();
    AssertThat( test_clock->last_set_offset, Equals( test_synchronizer->clock_offset() ) );
  }

  It( should_parse_only_time_sync_messages )
  {
    the::net::Data malformed_answer_message( create_answer_message() );
    malformed_answer_message[ 0 ] = 123;
    test_synchronizer->on_message_from_network( malformed_answer_message );

    AssertThat( test_synchronizer->clock_offset(), Equals( 0 ) );
    AssertThat( test_synchronizer->network_latency(), Equals( 0u ) );
  }

  It( should_not_crash_on_empty_message )
  {
    test_synchronizer->on_message_from_network( the::net::Data() );
  }

  It( should_drop_malformed_messages )
  {
    the::net::Data malformed_answer_message( create_answer_message() );
    malformed_answer_message.erase( end( malformed_answer_message ) - 2 );
    test_synchronizer->on_message_from_network( malformed_answer_message );

    AssertThat( test_synchronizer->clock_offset(), Equals( 0 ) );
    AssertThat( test_synchronizer->network_latency(), Equals( 0u ) );
  }

  const uint64_t init_time_on_client_side{ 1000 };
  const uint64_t response_time_on_server_side{ 200000 };
  const uint64_t answer_arrive_time_client_side{ 1500 };
  std::unique_ptr< TestClock > test_clock;
  std::unique_ptr< TestConnection > test_connection;
  typedef yarrr::clock_sync::Client< TestClock, TestConnection > TestSynchronizer;
  std::unique_ptr< TestSynchronizer > test_synchronizer;
};

Describe(a_server_clock_synchronizer)
{
  void SetUp()
  {
    test_clock.reset( new TestClock() );
    test_clock->set_time( response_time_on_server_side );
    test_connection.reset( new TestConnection() );
    test_synchronizer.reset( new TestSynchronizer( *test_clock, *test_connection ) );
  }

  the::net::Data create_request_message()
  {
    the::net::Data request;
    yarrr::Serializer serializer( request );
    serializer.push_back( yarrr::clock_sync::protocol_id );
    serializer.push_back( init_time_on_client_side );
    return request;
  }

  It( answers_to_clock_sync_requests )
  {
    test_synchronizer->on_message_from_network( create_request_message() );
    AssertThat( test_connection->message_was_sent, Equals( true ) );
  }

  It( drops_empty_messages )
  {
    test_synchronizer->on_message_from_network( the::net::Data() );
    AssertThat( test_connection->message_was_sent, Equals( false ) );
  }

  It( drops_malformed_messages )
  {
    the::net::Data malformed_message( create_request_message() );
    malformed_message.erase( end( malformed_message ) - 2 );
    test_synchronizer->on_message_from_network( malformed_message );
    AssertThat( test_connection->message_was_sent, Equals( false ) );
  }

  It( parses_only_time_sync_messages )
  {
    the::net::Data malformed_message( create_request_message() );
    malformed_message[ 0 ] = 200;
    test_synchronizer->on_message_from_network( malformed_message );
    AssertThat( test_connection->message_was_sent, Equals( false ) );
  }

  It( sends_back_the_original_timestamp_and_the_server_time )
  {
    test_synchronizer->on_message_from_network( create_request_message() );
    AssertThat( test_connection->message_was_sent, Equals( true ) );
    AssertThat( test_connection->init_time_sent_in_last_message, Equals( init_time_on_client_side ) );
    AssertThat( test_connection->response_time_sent_in_last_message, Equals( response_time_on_server_side ) );
  }

  const uint64_t init_time_on_client_side{ 1000 };
  const uint64_t response_time_on_server_side{ 200000 };
  std::unique_ptr< TestClock > test_clock;
  std::unique_ptr< TestConnection > test_connection;
  typedef yarrr::clock_sync::Server< TestClock, TestConnection > TestSynchronizer;
  std::unique_ptr< TestSynchronizer > test_synchronizer;
};

