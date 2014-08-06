#pragma once
#include <yarrr/object.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/ship_control.hpp>
#include <thectci/id.hpp>
#include <thetime/clock.hpp>

namespace yarrr
{

//todo: clean up after new object system is implemented
class LocalPhysicalBehavior : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_local_physical_behavior" );
    virtual void register_to(
        the::ctci::Dispatcher&,
        the::ctci::ComponentRegistry& registry ) override;

    PhysicalParameters physical_parameters;

    virtual Pointer clone() const override
    {
      return Pointer( new LocalPhysicalBehavior() );
    }

  private:
    virtual void do_serialize( yarrr::Serializer& serializer ) const override {}
    virtual void do_deserialize( yarrr::Deserializer& deserializer ) override {}
};

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

class SimplePhysicsUpdater : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_simple_physics_updater" );
    SimplePhysicsUpdater();

    virtual void register_to(
        the::ctci::Dispatcher&,
        the::ctci::ComponentRegistry& registry ) override;

    virtual Pointer clone() const override
    {
      return Pointer( new SimplePhysicsUpdater() );
    }
  private:
    void handle_timer_update( const TimerUpdate& ) const;
    LocalPhysicalBehavior* m_local_physical_behavior;

    virtual void do_serialize( yarrr::Serializer& serializer ) const override {}
    virtual void do_deserialize( yarrr::Deserializer& deserializer ) override {}
};

class ObjectStateUpdate;

class NetworkSynchronizer : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_network_synchronizer" );
    NetworkSynchronizer();

    virtual void register_to(
        the::ctci::Dispatcher&,
        the::ctci::ComponentRegistry& registry ) override;

    virtual Pointer clone() const override
    {
      return Pointer( new NetworkSynchronizer() );
    }
  private:
    void handle_object_state_update( const ObjectStateUpdate& ) const;
    LocalPhysicalBehavior* m_local_physical_behavior;

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

    virtual Pointer clone() const override
    {
      return Pointer( new Engine() );
    }
  private:
    virtual void do_serialize( yarrr::Serializer& serializer ) const override {}
    virtual void do_deserialize( yarrr::Deserializer& deserializer ) override {}
    std::unique_ptr< ShipControl > m_ship_control;
};


class SerializePhysicalParameter
{
  public:
    add_ctci( "serialize" );
    typedef std::vector< Data > SerializedDataBuffer;

    SerializePhysicalParameter( SerializedDataBuffer& serialized_data_buffer )
      : data_buffer( serialized_data_buffer )
    {
    }

    SerializedDataBuffer& data_buffer;
};

class PhysicalParameterSerializer : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_physical_parameter_serializer" );
    PhysicalParameterSerializer();

    virtual void register_to(
        the::ctci::Dispatcher&,
        the::ctci::ComponentRegistry& registry ) override;

    virtual Pointer clone() const override
    {
      return Pointer( new PhysicalParameterSerializer() );
    }
  private:
    virtual void do_serialize( yarrr::Serializer& serializer ) const override {}
    virtual void do_deserialize( yarrr::Deserializer& deserializer ) override {}
    void handle_serialize( const SerializePhysicalParameter& ) const;
    LocalPhysicalBehavior* m_local_physical_behavior;
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

