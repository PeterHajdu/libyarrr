#pragma once

#include <yarrr/event_factory.hpp>
#include <yarrr/types.hpp>
#include <thectci/dispatcher.hpp>
#include <memory>

namespace yarrr
{

template < typename Connection >
class ConnectionWrapper
{
  public:
    typedef std::unique_ptr< ConnectionWrapper > Pointer;
    ConnectionWrapper( Connection& connection )
      : connection( connection )
    {
    }

    void process_incoming_messages() const
    {
      yarrr::Data message;
      while( connection.receive( message ) )
      {
        Event::Pointer event( EventFactory::create( message ) );
        for ( auto& dispatcher : m_dispatchers )
        {
          dispatcher.get().polymorphic_dispatch( *event );
        }
      }
    }

    void register_dispatcher( the::ctci::Dispatcher& dispatcher )
    {
      m_dispatchers.push_back( dispatcher );
    }

    Connection& connection;

  private:
    typedef std::reference_wrapper< the::ctci::Dispatcher > DispatcherReference;
    std::vector< DispatcherReference > m_dispatchers;
};

}

