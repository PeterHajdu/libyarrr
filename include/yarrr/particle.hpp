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

    bool is_alive() const;

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

    //particle factory should take the ownership of the created particle
    virtual void create( const PhysicalParameters& ) = 0;
};


class ParticleContainer
{
  public:
    typedef size_t size_type;
    void push_back( Particle::Pointer&& );
    void travel_in_time_to( const the::time::Time& );
    size_type size() const;

  private:
    typedef std::vector< Particle::Pointer > Particles;
    Particles m_particles;
};

}

