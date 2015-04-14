#pragma once

#include <yarrr/entity_factory.hpp>
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
    ConnectionWrapper( typename Connection::Pointer connection )
      : connection( connection )
    {
    }

    void process_incoming_messages() const
    {
      yarrr::Data message;
      while( connection->receive( message ) )
      {
        Entity::Pointer entity( EntityFactory::create( message ) );

        if ( !entity )
        {
          connection->drop();
          return;
        }

        polymorphic_dispatch( *entity );
      }
    }

    typename Connection::Pointer connection;

    ConnectionWrapper( const ConnectionWrapper& ) = delete;
    ConnectionWrapper& operator=( const ConnectionWrapper& ) = delete;
    ConnectionWrapper( ConnectionWrapper&& ) = delete;
    ConnectionWrapper& operator=( ConnectionWrapper&& ) = delete;
};

}

