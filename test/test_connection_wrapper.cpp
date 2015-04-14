#include <yarrr/connection_wrapper.hpp>
#include <yarrr/types.hpp>
#include <yarrr/entity_factory.hpp>
#include <thectci/dispatcher.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{
  class TestEntity : public yarrr::Entity
  {
    public:
      add_polymorphic_ctci( "connection_wrapper_test_entity" );
  };

  yarrr::AutoEntityRegister<TestEntity> connection_wrapper_test_entity_register;

  class TestConnection
  {
    public:
      using Pointer = std::shared_ptr< TestConnection >;

      bool has_one_invalid_entity{ false };
      ssize_t number_of_messages{ 0 };
      bool receive( yarrr::Data& data )
      {
        if ( has_one_invalid_entity )
        {
          has_one_invalid_entity = false;
          return true;
        }

        if ( empty() )
        {
          return false;
        }

        data = TestEntity().serialize();
        --number_of_messages;
        return true;
      }

      bool empty() const
      {
        return 0 >= number_of_messages;
      }

       bool was_dropped{ false };
       void drop()
       {
         was_dropped = true;
       }
  };

  typedef yarrr::ConnectionWrapper< TestConnection > TestWrapper;

}

Describe(a_connection_wrapper)
{
  void SetUp()
  {
    test_connection = std::make_shared< TestConnection >();
    test_connection->number_of_messages = number_of_entities;

    test_wrapper = std::make_unique< TestWrapper >( test_connection );

    number_of_test_entity_dispatches_1 = 0;
    test_wrapper->register_dispatcher( test_dispatcher_1 );

    test_dispatcher_1.register_listener<TestEntity>(
        [ this ]( const TestEntity& )
        {
          ++number_of_test_entity_dispatches_1;
        } );

    number_of_test_entity_dispatches_2 = 0;
    test_wrapper->register_dispatcher( test_dispatcher_2 );

    test_dispatcher_2.register_listener<TestEntity>(
        [ this ]( const TestEntity& )
        {
          ++number_of_test_entity_dispatches_2;
        } );
  }

  It( exposes_the_wrapped_connection )
  {
    AssertThat( test_wrapper->connection.get(), Equals( test_connection.get() ) );
  }

  It( processes_all_waiting_messages )
  {
    test_wrapper->process_incoming_messages();
    AssertThat( test_connection->empty(), Equals( true ) );
  }

  It( forwards_messages_to_registered_dispatchers )
  {
    test_wrapper->process_incoming_messages();
    AssertThat( number_of_test_entity_dispatches_1, Equals( number_of_entities ) );
    AssertThat( number_of_test_entity_dispatches_2, Equals( number_of_entities ) );
  }

  It( drops_invalid_entities )
  {
    test_connection->has_one_invalid_entity = true;
    test_wrapper->process_incoming_messages();
  }

  It( drops_connection_if_invalid_entity_is_received )
  {
    test_connection->has_one_invalid_entity = true;
    test_wrapper->process_incoming_messages();
    AssertThat( test_connection->was_dropped, Equals( true ) );
  }

  const size_t number_of_entities{ 10 };
  size_t number_of_test_entity_dispatches_1{ 0 };
  size_t number_of_test_entity_dispatches_2{ 0 };
  std::shared_ptr< TestConnection > test_connection;
  TestWrapper::Pointer test_wrapper;
  the::ctci::Dispatcher test_dispatcher_1;
  the::ctci::Dispatcher test_dispatcher_2;
};

