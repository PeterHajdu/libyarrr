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

}

