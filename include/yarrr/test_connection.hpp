#pragma once

#include <yarrr/entity_factory.hpp>
#include <thenet/test_socket.hpp>
#include <thenet/connection.hpp>
#include <thenet/packetizer.hpp>

namespace test
{

class Connection
{
  public:
    using Pointer = std::shared_ptr< Connection >;

    Connection()
      : socket( test::Socket::create() )
      , connection( std::make_shared< the::net::Connection >( *socket ) )
      , wrapper( connection )
      , m_packetizer( *this )
    {
    }

    void process_messages()
    {
      connection->wake_up_on_network_thread();
      const yarrr::Data buffer( socket->sent_message() );
      m_packetizer.receive( &buffer[0], buffer.size() );
    }

    bool has_no_data()
    {
      process_messages();
      return sent_messages.empty();
    }

    template < typename EntityType >
    std::unique_ptr< EntityType > get_entity()
    {
      process_messages();
      for ( const auto& message : sent_messages )
      {
        yarrr::Entity::Pointer entity( yarrr::EntityFactory::create( message ) );
        if ( EntityType::ctci != entity->polymorphic_ctci() )
        {
          continue;
        }

        return std::unique_ptr<EntityType>(
            static_cast< EntityType* >( entity.release() ) );
      }

      return nullptr;
    }

    template < typename EntityType >
    std::vector< std::unique_ptr< EntityType > > entities()
    {
      std::vector< std::unique_ptr< EntityType > > entities;

      process_messages();
      for ( const auto& message : sent_messages )
      {
        yarrr::Entity::Pointer entity( yarrr::EntityFactory::create( message ) );
        if ( EntityType::ctci != entity->polymorphic_ctci() )
        {
          continue;
        }

        entities.emplace_back( static_cast< EntityType* >( entity.release() ) );
      }

      return entities;
    }

    template < typename EntityType >
    bool has_entity()
    {
      return get_entity< EntityType >().get();
    }

    std::vector< yarrr::Data > sent_messages;
    void message_from_network( yarrr::Data&& message )
    {
      sent_messages.push_back( message );
    }

    void drop()
    {
    }

    void flush_connection()
    {
      process_messages();
      sent_messages.clear();
    }

    std::unique_ptr< test::Socket > socket;
    the::net::Connection::Pointer connection;
    yarrr::ConnectionWrapper< the::net::Connection > wrapper;

  private:
    the::net::packetizer::Incoming< Connection > m_packetizer;
};

}

