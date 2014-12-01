#include <yarrr/object_decorator.hpp>
#include <yarrr/object.hpp>
#include <yarrr/types.hpp>
#include <yarrr/log.hpp>
#include <yarrr/basic_behaviors.hpp>

namespace yarrr
{

ObjectDecorator::ObjectDecorator( Object& object )
  : m_object( object )
{
}

std::string
ObjectDecorator::id()
{
  return std::to_string( m_object.id() );
}


void
ObjectDecorator::add_behavior_clone( ObjectBehavior& behavior )
{
  thelog( yarrr::log::debug )( "Adding behavior clone to:", &m_object, m_object.id(), "behavior:", &behavior );
  m_object.add_behavior( behavior.clone() );
}

void
ObjectDecorator::move_to( const yarrr::Coordinate& new_position )
{
  coordinate_of( m_object ) = new_position;
}

void
ObjectDecorator::set_velocity( const yarrr::Velocity& new_velocity )
{
  velocity_of( m_object ) = new_velocity;
}

}

