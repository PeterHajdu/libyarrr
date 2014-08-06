#include <yarrr/object.hpp>
#include <yarrr/entity.hpp>
#include <yarrr/entity_factory.hpp>
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
    add_polymorphic_ctci( "yarrr__test_behavior" );
    bool was_registered{ false };
    bool test_behavior_was_already_registered{ false };
    size_t number_of_test_behavior_registrations{ 1 };
    virtual void register_to(
        the::ctci::Dispatcher& dispatcher,
        the::ctci::ComponentRegistry& components ) override
    {
      was_registered = true;
      dispatcher.register_listener<TestEvent>( std::bind(
            &TestBehavior::handle_test_event, this, std::placeholders::_1 ) );

      dispatcher.register_listener<TestEventChild>( std::bind(
            &TestBehavior::handle_test_event_child, this, std::placeholders::_1 ) );

      test_behavior_was_already_registered = components.has_component< TestBehavior >();
      if ( test_behavior_was_already_registered )
      {
        number_of_test_behavior_registrations =
          ++components.component< TestBehavior >().number_of_test_behavior_registrations;
      }
      components.register_component( *this );
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

    virtual ObjectBehavior::Pointer clone() const override
    {
      return ObjectBehavior::Pointer( new TestBehavior() );
    }

  private:
    virtual void do_serialize( yarrr::Serializer& serializer ) const override
    {
    }

    virtual void do_deserialize( yarrr::Deserializer& deserializer ) override
    {
    }
};

yarrr::AutoEntityRegister< TestBehavior > register_test_behavior;

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

  It( is_identified_by_its_pointer_if_default_constructed )
  {
    yarrr::Object one_object;
    AssertThat( one_object.id, Equals( yarrr::Object::Id( &one_object ) ) );
  }

  It( can_be_constructed_with_a_specific_id )
  {
    yarrr::Object one_object( 123u );
    AssertThat( one_object.id, Equals( 123u ) );
  }

  yarrr::Object::Pointer test_object;
  TestBehavior* test_behavior;
};

Describe( an_object_update )
{

  void serialize_and_deserialize()
  {
    yarrr::Data serialized_update( test_update->serialize() );
    deserialized_entity = yarrr::EntityFactory::create( serialized_update );
    deserialized_update = static_cast< const yarrr::ObjectUpdate* >( deserialized_entity.get() );
    recreated_object = deserialized_update->create_object();
  }

  void SetUp()
  {
    assert( yarrr::EntityFactory::is_registered( TestBehavior::ctci ) );
    test_object.reset( new yarrr::Object() );
    test_behavior = new TestBehavior();
    test_object->add_behavior( yarrr::ObjectBehavior::Pointer( new TestBehavior() ) );
    test_object->add_behavior( yarrr::ObjectBehavior::Pointer( test_behavior ) );
    test_entity = test_object->generate_update();
    test_update = static_cast< yarrr::ObjectUpdate* >( test_entity.get() );
    serialize_and_deserialize();
  }

  It( has_the_same_id_as_the_object )
  {
    AssertThat( test_update->id(), Equals( test_object->id ) );
  }

  It( is_registered_to_entity_factory )
  {
    AssertThat( yarrr::EntityFactory::is_registered( yarrr::ObjectUpdate::ctci ), Equals( true ) );
  }

  It( serializes_and_deserializes_the_id )
  {
    AssertThat( deserialized_update->id(), Equals( test_object->id ) );
  }

  It( can_create_new_objects_with_the_same_id )
  {
    AssertThat( recreated_object->id, Equals( test_object->id ) );
  }

  It( can_create_new_objects_with_the_same_behaviors )
  {
    TestBehavior* behavior_spy( new TestBehavior() );
    recreated_object->add_behavior( yarrr::ObjectBehavior::Pointer( behavior_spy ) );
    AssertThat( behavior_spy->number_of_test_behavior_registrations, Equals( 3 ) );
  }

  yarrr::Object::Pointer recreated_object;
  yarrr::Entity::Pointer test_entity;
  yarrr::Entity::Pointer deserialized_entity;
  const yarrr::ObjectUpdate* deserialized_update;
  const yarrr::ObjectUpdate* test_update;
  yarrr::Object::Pointer test_object;
  TestBehavior* test_behavior;
};

