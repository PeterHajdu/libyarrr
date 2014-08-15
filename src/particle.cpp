#include <yarrr/particle.hpp>

namespace
{
  const the::time::Time two_seconds{ 2000000 };
}

namespace yarrr
{

Particle::Particle( const PhysicalParameters& physical_parameters, GraphicalEngine& graphical_engine )
  : GraphicalObject( graphical_engine )
  , m_physical_parameters( physical_parameters )
  , m_time_of_birth( physical_parameters.timestamp )
{
}

uint64_t
Particle::age() const
{
  return m_physical_parameters.timestamp - m_time_of_birth;
}

void
Particle::draw() const
{
  m_graphical_engine.draw_particle( m_physical_parameters, age() );
}

void
Particle::travel_in_time_to( const the::time::Time& timepoint )
{
  yarrr::travel_in_time_to( timepoint, m_physical_parameters );
}

bool
Particle::is_alive() const
{
  return age() < two_seconds;
  return true;
}


void
ParticleContainer::push_back( Particle::Pointer&& particle )
{
  m_particles.emplace_back( std::move( particle ) );
}

void
ParticleContainer::travel_in_time_to( const the::time::Time& timepoint )
{
  for ( auto& particle : m_particles )
  {
    particle->travel_in_time_to( timepoint );
  }

  Particles::iterator first_alive( std::begin( m_particles ) );
  while( first_alive != m_particles.end() && !(*first_alive)->is_alive() )
  {
    ++first_alive;
  }

  m_particles.erase( std::begin( m_particles ), first_alive );
}

ParticleContainer::size_type
ParticleContainer::size() const
{
  return m_particles.size();
}

}

