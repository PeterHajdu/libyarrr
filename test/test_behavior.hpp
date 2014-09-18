#pragma once
#include "test_events.hpp"
#include <yarrr/object.hpp>
#include <yarrr/bitmagic.hpp>

namespace test
{

class Behavior : public yarrr::ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_another_test_behavior" );
    Behavior( int synchronization_period = yarrr::always_synchronize(), std::function< void() > call_when_deleted = [](){} )
      : ObjectBehavior( synchronization_period )
      , some_data( id() + 1 )
      , m_call_when_deleted( call_when_deleted )
    {
    }

    Behavior( Id id )
      : ObjectBehavior( yarrr::always_synchronize(), id )
      , some_data( id + 1 )
      , m_call_when_deleted( [](){} )
    {
    }

    size_t number_of_test_behavior_registrations{ 1 };
    bool was_registered{ false };

    const test::Event* dispatched_event{ nullptr };
    void handle_event( const test::Event& event )
    {
      dispatched_event = &event;
    }

    virtual ~Behavior()
    {
      m_call_when_deleted();
    }

    mutable bool was_cloned{ false };
    virtual yarrr::ObjectBehavior::Pointer clone() const override
    {
      was_cloned=true;
      return yarrr::ObjectBehavior::Pointer( new Behavior( id() ) );
    }

    const ObjectBehavior* updated_with{ nullptr };
    virtual void update( const ObjectBehavior& behavior )
    {
      updated_with = &behavior;
    }

    bool was_updated() const
    {
      return updated_with != nullptr;
    }

    Id some_data;

  private:
    virtual void do_register_to( yarrr::Object& owner ) override
    {
      owner.dispatcher.register_listener< test::Event >( std::bind(
            &Behavior::handle_event, this, std::placeholders::_1 ) );
      was_registered = true;

      const bool was_already_registered( owner.components.has_component< Behavior >() );
      if ( was_already_registered )
      {
        number_of_test_behavior_registrations =
          ++owner.components.component< Behavior >().number_of_test_behavior_registrations;
      }
    }

    virtual void serialize_behavior( yarrr::Serializer& serializer ) const
    {
      serializer.push_back( some_data );
    }

    virtual void deserialize_behavior( yarrr::Deserializer& deserializer )
    {
      some_data = deserializer.pop_front< Id >();
    }

    std::function< void() > m_call_when_deleted;
};


class NonSynchronizableBehavior : public yarrr::ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_nonsynchronizable_behavior" );
    NonSynchronizableBehavior()
      : ObjectBehavior( yarrr::do_not_synchronize() )
    {
    }

    mutable bool was_cloned{ false };
    virtual yarrr::ObjectBehavior::Pointer clone() const
    {
      was_cloned = true;
      return Pointer{ nullptr };
    }

  private:
    virtual void do_register_to( yarrr::Object& ) override
    {
    }

};

class NthSynchronizedBehavior : public yarrr::ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_nth_synchronized_behavior" );
    NthSynchronizedBehavior( int n )
      : ObjectBehavior( n )
    {
    }

    virtual yarrr::ObjectBehavior::Pointer clone() const
    {
      return Pointer{ nullptr };
    }
};

}

