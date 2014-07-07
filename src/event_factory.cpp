#include <yarrr/event_factory.hpp>
#include <yarrr/bitmagic.hpp>
#include <thectci/id.hpp>
#include <memory>

namespace
{
  yarrr::EventFactory* instance;
}

namespace yarrr
{

EventFactory&
EventFactory::get()
{
  if ( !instance )
  {
    instance = new yarrr::EventFactory();
  }

  return *instance;
}


Event::Pointer
EventFactory::create( const Data& data )
{
  EventFactory& factory( get() );
  Event::Pointer event( factory.m_factory.create( extract< the::ctci::Id >( &data[0] ) ) );
  if ( !event )
  {
    return event;
  }
  event->deserialize( data );
  return event;
}


bool
EventFactory::is_registered( the::ctci::Id id )
{
  return get().m_factory.is_registered( id );
}

}

