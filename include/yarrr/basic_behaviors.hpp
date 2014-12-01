#pragma once
#include <yarrr/object.hpp>
#include <yarrr/physical_parameters.hpp>
#include <thectci/id.hpp>
#include <thetime/clock.hpp>
#include <yarrr/graphical_engine.hpp>

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
    PhysicalBehavior();
    PhysicalBehavior( const PhysicalParameters& );
    PhysicalBehavior( const PhysicalParameters&, const Id& );

    PhysicalParameters physical_parameters;
    virtual Pointer clone() const override;
    virtual ~PhysicalBehavior() = default;
    virtual void update( const ObjectBehavior& ) override;
  private:
    virtual void do_register_to( Object& ) override;

    void handle_timer_update( const TimerUpdate& );

    virtual void serialize_behavior( Serializer& serializer ) const override;
    virtual void deserialize_behavior( Deserializer& deserializer ) override;
};

Coordinate& coordinate_of( const Object& );
Velocity& velocity_of( const Object& );

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
    GraphicalBehavior( const Id& id );

    Pointer clone() const override = 0;
    virtual void draw() const override = 0;

  protected:
    PhysicalBehavior* m_physical_behavior;

  private:
    void handle_focus_on_object( const FocusOnObject& );
    virtual void do_register_to( Object& ) override;
    virtual void register_graphical_behavior_to( Object& ) {};
};

class ShapeGraphics : public GraphicalBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_shape_graphics" );
    ShapeGraphics() = default;
    ShapeGraphics( const Id& id );
    virtual void draw() const override;
    Pointer clone() const override;

  private:
    virtual void register_graphical_behavior_to( Object& ) override;
};

class LaserGraphics : public GraphicalBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_laser_graphics" );
    LaserGraphics() = default;
    LaserGraphics( const Id& id );
    virtual void draw() const;
    Pointer clone() const;
};

}

