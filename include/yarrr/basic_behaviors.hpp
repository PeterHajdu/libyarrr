#pragma once
#include <yarrr/object.hpp>
#include <yarrr/physical_parameters.hpp>
#include <thectci/id.hpp>
#include <thetime/clock.hpp>
#include <yarrr/graphical_engine.hpp>

namespace yarrr
{

class ObjectCreated
{
  public:
    add_ctci( "yarrr_object_created" );

    ObjectCreated( Object::Pointer&& object )
      : object( std::move( object ) )
    {
    }

    mutable Object::Pointer object;
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

class PhysicalBehavior : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_physical_behavior" );
    PhysicalBehavior();
    PhysicalBehavior( const PhysicalParameters& );

    PhysicalParameters physical_parameters;
    virtual Pointer clone() const override;
    virtual ~PhysicalBehavior() = default;
  private:
    virtual void do_register_to( Object& ) override;

    void handle_timer_update( const TimerUpdate& );
    void handle_network_update( const PhysicalBehavior& );

    virtual void do_serialize( Serializer& serializer ) const override;
    virtual void do_deserialize( Deserializer& deserializer ) override;
};

class SelfDestructor : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_self_destructor" );
    SelfDestructor(
        Object::Id object_id,
        const the::time::Time& lifespan );

    virtual Pointer clone() const override;

  private:
    virtual void do_register_to( Object& ) override;

    void handle_timer_update( const TimerUpdate& );

    the::time::Time m_lifespan;
    Object::Id m_object_id;

    the::time::Time m_time_to_die;
};

class FocusOnObject
{
  public:
    add_ctci( "yarrr_focus_on_object" )
};

class GraphicalBehavior :
  public ObjectBehavior,
  public GraphicalObject
{
  public:
    GraphicalBehavior();

    Pointer clone() const = 0;
    virtual void draw() const = 0;

  protected:
    PhysicalBehavior* m_physical_behavior;

  private:
    void handle_focus_on_object( const FocusOnObject& );
    virtual void do_register_to( Object& ) override;
};

class ShipGraphics : public GraphicalBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_ship_graphics" );
    virtual void draw() const;
    Pointer clone() const;
};

class LaserGraphics : public GraphicalBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_laser_graphics" );
    virtual void draw() const;
    Pointer clone() const;
};

}

