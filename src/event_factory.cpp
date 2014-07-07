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
  event->deserialize( data );
  return event;
}

void
EventFactory::destroy()
{
  delete instance;
  instance = nullptr;
}

}

