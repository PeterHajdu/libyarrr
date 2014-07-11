#include <yarrr/object.hpp>
#include <thectci/dispatcher.hpp>
#include <thectci/component_registry.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{

class TestEvent
{
  public:
    add_polymorphic_ctci( "test_event" );
};

class TestEventChild : public TestEvent
{
  public:
    add_polymorphic_ctci( "test_event_child" );
};

class TestBehavior : public yarrr::ObjectBehavior
{
  public:
    bool was_registered{ false };
    virtual void register_to(
        the::ctci::Dispatcher& dispatcher,
        the::ctci::ComponentRegistry& ) override
    {
      was_registered = true;
      dispatcher.register_listener<TestEvent>( std::bind(
            &TestBehavior::handle_test_event, this, std::placeholders::_1 ) );

      dispatcher.register_listener<TestEventChild>( std::bind(
            &TestBehavior::handle_test_event_child, this, std::placeholders::_1 ) );
    }

    const TestEvent* dispatched_event{ nullptr };
    void handle_test_event( const TestEvent& event )
    {
      dispatched_event = &event;
    }

    const TestEvent* dispatched_child_event{ nullptr };
    void handle_test_event_child( const TestEventChild& event )
    {
      dispatched_child_event = &event;
    }

    bool was_deleted{ false };
    virtual ~TestBehavior()
    {
      was_deleted = true;
    }
};

}

Describe(an_object)
{

  void SetUp()
  {
    test_object.reset( new yarrr::Object() );
    test_behavior = new TestBehavior();
    test_object->add_behavior( yarrr::ObjectBehavior::Pointer( test_behavior ) );
    AssertThat( test_behavior->was_deleted, Equals( false ) );
  }

  It( registers_behavior )
  {
    AssertThat( test_behavior->was_registered, Equals( true ) );
  }

  It( dispatches_events_to_registered_listeners )
  {
    TestEvent test_event;
    test_object->dispatch( test_event );
    AssertThat( test_behavior->dispatched_event, Equals( &test_event ) );
  }

  It( dispatches_by_polymorphic_id_if_requested )
  {
    std::unique_ptr< TestEvent > test_event( new TestEventChild() );
    test_object->polymorphic_dispatch( *test_event );
    AssertThat( nullptr == test_behavior->dispatched_event, Equals( true ) );
    AssertThat( test_behavior->dispatched_child_event, Equals( test_event.get() ) );
  }

  yarrr::Object::Pointer test_object;
  TestBehavior* test_behavior;
};

