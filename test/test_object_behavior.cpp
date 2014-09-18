#include <yarrr/object_behavior.hpp>
#include "test_behavior.hpp"
#include <yarrr/object_update.hpp>
#include <thectci/component_registry.hpp>
#include <thetime/timer.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;


Describe( an_object_behavior )
{
  void SetUp()
  {
    object.reset( new yarrr::Object() );
    test_behavior = new test::Behavior();
    behavior.reset( test_behavior );
    behavior->register_to( *object );
    serialized_data = behavior->serialize();
    deserialized_behavior.reset( new test::Behavior() );
    deserialized_behavior->deserialize( serialized_data );
  }

  It( registers_child_objects )
  {
    AssertThat( test_behavior->was_registered, Equals( true ) );
  }

  It( registers_child_as_a_component )
  {
    AssertThat( object->components.has_component< test::Behavior >(), Equals( true ) );
  }

  It( has_an_id )
  {
    AssertThat( behavior->id(), IsGreaterThan( 0u ) );
  }

  It( serializes_and_deserializes_the_same_id )
  {
    AssertThat( deserialized_behavior->id(), Equals( behavior->id() ) );
  }

  It( calls_child_class_serializer_and_deserializer )
  {
    AssertThat( deserialized_behavior->some_data, Equals( test_behavior->some_data ) );
  }

  yarrr::Data serialized_data;
  yarrr::Object::Pointer object;
  yarrr::ObjectBehavior::Pointer behavior;
  test::Behavior* test_behavior;

  std::unique_ptr< test::Behavior > deserialized_behavior;
};

Describe( behavior_synchronization )
{
  It( should_always_synchronize_behaviors_marked_so )
  {
    typedef test::Behavior AlwaysSynchronizingBehavior;
    AlwaysSynchronizingBehavior behavior;
    10_times( [ &behavior ]() {
        AssertThat( behavior.should_synchronize(), Equals( true ) ); } );
  }

  It( should_never_synchronize_behaviors_marked_so )
  {
    test::NonSynchronizableBehavior behavior;
    10_times( [ &behavior ]() {
        AssertThat( behavior.should_synchronize(), Equals( false ) ); } );
  }

  It( should_synchronize_every_nth_of_a_periodically_synchronized_behavior )
  {
    test::NthSynchronizedBehavior behavior( 2 );
    10_times( [ &behavior ]() {
        AssertThat( behavior.should_synchronize(), Equals( true ) );
        AssertThat( behavior.should_synchronize(), Equals( false ) );
        } );
  }

  It( should_synchronize_when_forced )
  {
    test::NthSynchronizedBehavior behavior( 2 );

    AssertThat( behavior.should_synchronize(), Equals( true ) );
    behavior.force_synchronization();

    10_times( [ &behavior ]() {
        AssertThat( behavior.should_synchronize(), Equals( true ) );
        AssertThat( behavior.should_synchronize(), Equals( false ) );
        } );
  }

};

