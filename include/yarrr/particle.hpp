#pragma once

#include <yarrr/physical_parameters.hpp>
#include <yarrr/graphical_engine.hpp>
#include <memory>

namespace yarrr
{


class Particle : public GraphicalObject
{
  public:
    typedef std::unique_ptr< Particle > Pointer;

    Particle( const PhysicalParameters&, GraphicalEngine& );
    virtual ~Particle() = default;

    virtual void draw() const override;
    void travel_in_time_to( const the::time::Time& );

  private:
    uint64_t age() const;
    PhysicalParameters m_physical_parameters;
    const the::time::Time m_time_of_birth;
};


class ParticleFactory
{
  public:
    add_ctci( "yarrr_particle_factory" );
    virtual ~ParticleFactory() = default;

    virtual Particle::Pointer create( const PhysicalParameters& ) = 0;
};

}

