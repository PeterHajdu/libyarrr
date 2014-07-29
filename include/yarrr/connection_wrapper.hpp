#pragma once

#include <yarrr/event_factory.hpp>
#include <yarrr/types.hpp>
#include <thectci/dispatcher.hpp>
#include <memory>

namespace yarrr
{

template < typename Connection >
class ConnectionWrapper : public the::ctci::Dispatcher
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

        if ( !event )
        {
          continue;
        }

        polymorphic_dispatch( *event );
      }
    }

    Connection& connection;

    ConnectionWrapper( const ConnectionWrapper& ) = delete;
    ConnectionWrapper& operator=( const ConnectionWrapper& ) = delete;
};

}

