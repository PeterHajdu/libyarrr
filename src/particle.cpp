#include <yarrr/particle.hpp>

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

}

