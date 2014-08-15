#pragma once

#include <yarrr/particle.hpp>

namespace test
{

class ParticleFactory : public yarrr::ParticleFactory
{
  public:
    virtual ~ParticleFactory() = default;

    void reset()
    {
      was_particle_created = false;
    }

    bool was_particle_created{ false };
    yarrr::PhysicalParameters last_particle_parameters;

    virtual void create( const yarrr::PhysicalParameters& physical_parameters ) override
    {
      was_particle_created = true;
      last_particle_parameters = physical_parameters;
    }
};

}

