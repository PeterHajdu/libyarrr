#pragma once

#include <yarrr/particle.hpp>
#include <yarrr/object.hpp>
#include <thectci/id.hpp>

namespace yarrr
{

class Command;
class ShipControl;
class PhysicalParameters;
class TimerUpdate;
class Serializer;
class Deserializer;

class Jet
{
  public:
    static const the::time::Time cooldown_time;

    Jet();
    void activate( const the::time::Time& at );
    bool is_active_at( const the::time::Time& at ) const;

    void serialize( Serializer& ) const;
    void deserialize( Deserializer& );

  private:
    the::time::Time m_active_until;
};

class Engine : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_engine" );

    Engine();
    virtual ~Engine();

    virtual void register_to( Object& ) override;

    virtual Pointer clone() const override;

  private:
    virtual void do_serialize( Serializer& serializer ) const;
    virtual void do_deserialize( Deserializer& deserializer );

    void handle_command( const yarrr::Command& );
    void handle_timer_update( const yarrr::TimerUpdate& ) const;
    std::unique_ptr< ShipControl > m_ship_control;
    PhysicalParameters* m_physical_parameters;
    const ParticleSource m_particle_source;

    Jet m_thruster;
};

}

