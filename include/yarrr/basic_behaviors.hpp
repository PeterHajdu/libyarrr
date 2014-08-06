#pragma once
#include <yarrr/object.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/ship_control.hpp>
#include <thectci/id.hpp>
#include <thetime/clock.hpp>

namespace yarrr
{

class TimerUpdate
{
  public:
    add_ctci( "timer_update" );
    TimerUpdate( const the::time::Time& timestamp )
      : timestamp( timestamp )
    {
    }

    const the::time::Time& timestamp;
};

class PhysicalBehavior : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_physical_behavior" );
    virtual void register_to(
        the::ctci::Dispatcher&,
        the::ctci::ComponentRegistry& registry ) override;

    PhysicalParameters physical_parameters;

    //todo: test clone
    virtual Pointer clone() const override
    {
      return Pointer( new PhysicalBehavior() );
    }

  private:
    void handle_timer_update( const TimerUpdate& );
    void handle_network_update( const PhysicalBehavior& );

    virtual void do_serialize( yarrr::Serializer& serializer ) const override {}
    virtual void do_deserialize( yarrr::Deserializer& deserializer ) override {}
};

class Command;
class ShipControl;

class Engine : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_engine" );
    virtual void register_to(
        the::ctci::Dispatcher&,
        the::ctci::ComponentRegistry& registry ) override;

    //todo: test clone
    virtual Pointer clone() const override
    {
      return Pointer( new Engine() );
    }
  private:
    virtual void do_serialize( yarrr::Serializer& serializer ) const override {}
    virtual void do_deserialize( yarrr::Deserializer& deserializer ) override {}
    std::unique_ptr< ShipControl > m_ship_control;
};


class ObjectContainer;
class Canon : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_canon" );
    Canon( ObjectContainer& object_container );

    virtual void register_to(
        the::ctci::Dispatcher&,
        the::ctci::ComponentRegistry& registry ) override;

    //todo: test clone
    virtual Pointer clone() const override
    {
      return Pointer( new Canon( m_object_container ) );
    }
  private:
    virtual void do_serialize( yarrr::Serializer& serializer ) const override {}
    virtual void do_deserialize( yarrr::Deserializer& deserializer ) override {}
    void handle_command( const Command& ) const;

    ObjectContainer& m_object_container;
};

}

