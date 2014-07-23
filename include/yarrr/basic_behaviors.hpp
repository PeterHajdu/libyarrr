#pragma once
#include <yarrr/object.hpp>
#include <yarrr/physical_parameters.hpp>
#include <thectci/id.hpp>
#include <thetime/clock.hpp>

namespace yarrr
{

class LocalPhysicalBehavior : public ObjectBehavior
{
  public:
    add_ctci( "local_physical_behavior" );
    virtual void register_to(
        the::ctci::Dispatcher&,
        the::ctci::ComponentRegistry& registry ) override;

    PhysicalParameters physical_parameters;
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
    SimplePhysicsUpdater();

    virtual void register_to(
        the::ctci::Dispatcher&,
        the::ctci::ComponentRegistry& registry ) override;

  private:
    void handle_timer_update( const TimerUpdate& ) const;
    LocalPhysicalBehavior* m_local_physical_behavior;
};

class ObjectStateUpdate;

class NetworkSynchronizer : public ObjectBehavior
{
  public:
    NetworkSynchronizer();

    virtual void register_to(
        the::ctci::Dispatcher&,
        the::ctci::ComponentRegistry& registry ) override;

  private:
    void handle_object_state_update( const ObjectStateUpdate& ) const;
    LocalPhysicalBehavior* m_local_physical_behavior;
};

class Command;
class ShipControl;

class Engine : public ObjectBehavior
{
  public:
    virtual void register_to(
        the::ctci::Dispatcher&,
        the::ctci::ComponentRegistry& registry ) override;

  private:
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
    PhysicalParameterSerializer();

    virtual void register_to(
        the::ctci::Dispatcher&,
        the::ctci::ComponentRegistry& registry ) override;

  private:
    void handle_serialize( const SerializePhysicalParameter& ) const;
    LocalPhysicalBehavior* m_local_physical_behavior;
};

}

