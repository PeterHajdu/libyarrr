#include <yarrr/object_container.hpp>
#include <yarrr/object.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{
  class Event
  {
    public:
      add_ctci( "a_test_event_for_object_container" );
  };

  class TestBehavior : public yarrr::ObjectBehavior
  {
    public:
      TestBehavior(
          std::function< void() > call_when_deleted,
          std::function< void( const Event& ) > call_when_event_dispatched )
        : m_call_when_deleted( call_when_deleted )
        , m_call_when_event_is_dispatched( call_when_event_dispatched )
      {
      }

      virtual void register_to(
          the::ctci::Dispatcher& dispatcher,
          the::ctci::ComponentRegistry& )
      {
        dispatcher.register_listener< Event >( std::bind(
              &TestBehavior::handle_event, this, std::placeholders::_1 ) );
      }

      void handle_event( const Event& event )
      {
        m_call_when_event_is_dispatched( event );
      }

      virtual ~TestBehavior()
      {
        m_call_when_deleted();
      }

    private:
      std::function< void() > m_call_when_deleted;
      std::function< void( const Event& ) > m_call_when_event_is_dispatched;
  };
}

Describe(an_object_container)
{

  void add_object_with_id( yarrr::Object::Id id )
  {
    yarrr::Object::Pointer new_object( new yarrr::Object() );
    new_object->add_behavior( yarrr::ObjectBehavior::Pointer(
          new TestBehavior(
            [ this, id ]()
            {
              deleted_objects.push_back( id );
            },
            [ this ]( const Event& event )
            {
              dispatched_events.push_back( &event );
            } ) ) );

    test_container->add_object( id, std::move( new_object ) );
  }

  void SetUp()
  {
    test_container.reset( new yarrr::ObjectContainer() );
    deleted_objects.clear();
    dispatched_events.clear();

    add_object_with_id( first_id );
    add_object_with_id( second_id );
  }

  It( stores_objects )
  {
    AssertThat( deleted_objects, IsEmpty() );
  }

  It( can_delete_objects_by_id )
  {
    test_container->delete_object( first_id );
    AssertThat( deleted_objects, Contains( first_id ) );
    AssertThat( deleted_objects, !Contains( second_id ) );
  }

  It( handles_invalid_id_deletion )
  {
    test_container->delete_object( invalid_id );
  }

  It( dispatches_events_to_all_objects )
  {
    test_container->dispatch( test_event );
    AssertThat( dispatched_events, Has().Exactly( 2 ).EqualTo( &test_event ) );
  }

  It( does_not_dispatch_to_deleted_objects )
  {
    test_container->delete_object( first_id );
    test_container->dispatch( test_event );
    AssertThat( dispatched_events, Has().Exactly( 1 ).EqualTo( &test_event ) );
  }

  It( can_tell_if_it_contains_an_object_or_not )
  {
    AssertThat( test_container->has_object_with_id( second_id ), Equals( true ) );
    AssertThat( test_container->has_object_with_id( invalid_id ), Equals( false ) );
  }

  It( can_retrieve_an_object_with_a_given_id )
  {
    test_container->object_with_id( second_id ).dispatch( test_event );
    AssertThat( dispatched_events, Has().Exactly( 1 ).EqualTo( &test_event ) );
  }

  Event test_event;
  std::vector< yarrr::Object::Id > deleted_objects;
  std::vector< const Event* > dispatched_events;
  const yarrr::Object::Id first_id{ 1 };
  const yarrr::Object::Id second_id{ 2 };
  const yarrr::Object::Id invalid_id{ 20 };
  std::unique_ptr< yarrr::ObjectContainer > test_container;
};

