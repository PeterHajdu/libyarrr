#include <yarrr/network.hpp>
#include <unistd.h>
#include <array>

//todo: remove iostream and inet.h
#include <iostream>
#include <arpa/inet.h>

namespace
{
  //todo: consider error handling of these system functions
  void allow_address_reuse( int socket )
  {
    const int true_value( 1 );
    setsockopt(
        socket,
        SOL_SOCKET,
        SO_REUSEADDR,
        &true_value,
        sizeof( true_value ) );
  }


  void bind_to_port( int port, int socket )
  {
    struct sockaddr_in address;
    memset( &address, 0, sizeof( address ) );
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( port );

    bind( socket, (struct sockaddr *) &address, sizeof( address ) );
  }


  void listen_with_queue_length( int queue_length, int socket )
  {
    ::listen( socket, queue_length );
  }



  class ReadingSocket : public yarrr::Socket
  {
    public:
      ReadingSocket( int socket )
        : Socket( socket )
      {
      }

      void handle_event() override
      {
        const size_t size_of_message( read( fd, &m_buffer[0], max_message_size ) );
        std::cout
          << "read " << size_of_message << " bytes -> ";
        std::copy(
            begin( m_buffer ), end( m_buffer ),
            std::ostream_iterator< char >( std::cout ) );

      }

    private:
      static const int max_message_size = 1000;
      std::array<char, max_message_size> m_buffer;
  };
}


namespace yarrr
{
  ListeningSocket::ListeningSocket( int port, AddSocketCallback add_socket_callback )
    : Socket( socket( AF_INET, SOCK_STREAM, 0 ) )
    , m_add_socket_callback( add_socket_callback )
  {
    allow_address_reuse( fd );
    bind_to_port( port, fd );
    listen_with_queue_length( 5, fd );
  }


  void
  ListeningSocket::handle_event()
  {
    struct sockaddr_in address;
    socklen_t sin_size = sizeof( address );

    std::cout << "connection from: " << inet_ntoa( address.sin_addr ) << std::endl;
    Socket::pointer new_socket( new ReadingSocket(
          accept(fd, (struct sockaddr *) &address, &sin_size) ) );
    m_add_socket_callback( std::move( new_socket ) );
  }


  Socket::Socket( int fd )
    : fd( fd )
  {
  }


  Socket::~Socket()
  {
    close( fd );
  }
}

