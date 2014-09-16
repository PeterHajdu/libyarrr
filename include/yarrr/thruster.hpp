#pragma once

#include <yarrr/particle.hpp>
#include <yarrr/item.hpp>
#include <yarrr/command.hpp>
#include <thectci/id.hpp>

namespace yarrr
{

class PhysicalParameters;
class TimerUpdate;
class Serializer;
class Deserializer;
class Shape;

class Jet
{
  public:
    static const the::time::Time cooldown_time;

    Jet();
    Jet( const Jet& );
    Jet& operator=( const Jet& );

    void activate( const the::time::Time& at );
    bool is_active_at( const the::time::Time& at ) const;

    void serialize( Serializer& ) const;
    void deserialize( Deserializer& );

  private:
    the::time::Time m_active_until;
};

class Thruster : public Item
{
  public:
    add_polymorphic_ctci( "yarrr_thruster" );

    Thruster();
    Thruster(
        Command::Type activation_command,
        const Coordinate& relative_coordinate,
        Angle direction );

    Thruster( const Thruster& );
    virtual ~Thruster();


    virtual Pointer clone() const override;
    virtual void update( const ObjectBehavior& ) override;

  private:
    virtual void register_item_to( Object& ) override;
    virtual void serialize_behavior( Serializer& serializer ) const;
    virtual void deserialize_behavior( Deserializer& deserializer );

    void apply_forces();
    void handle_command( const yarrr::Command& );
    void handle_timer_update( const yarrr::TimerUpdate& ) const;

    PhysicalParameters* m_physical_parameters;
    Shape* m_shape;
    const ParticleSource m_particle_source;

    Jet m_jet;
    Command::Type m_activation_command;
    Coordinate m_relative_coordinate;
    Coordinate m_normalized_relative_coordinate;
    Angle m_direction;
};

}

