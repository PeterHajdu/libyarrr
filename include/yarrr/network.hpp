#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <poll.h>
#include <algorithm>
#include <memory>


namespace yarrr
{
  class Socket
  {
    public:
      typedef std::unique_ptr< Socket > pointer;
      Socket( int fd );
      virtual ~Socket();

      const int fd;
      virtual void handle_event() = 0;
  };

  class ListeningSocket : public Socket
  {
    public:
      typedef std::function<void(Socket::pointer&&)> AddSocketCallback;
      ListeningSocket( int port, AddSocketCallback add_socket_callback );

      virtual void handle_event() override;

    private:
      AddSocketCallback m_add_socket_callback;
  };

  template < typename MessageFactory >
  class SocketPool
  {
    typedef SocketPool<MessageFactory> my_type;
    public:
      SocketPool()
      {
      }

      void listen( int port )
      {
        add_socket( Socket::pointer(
              new ListeningSocket(
                port,
                std::bind( &my_type::add_socket, this, std::placeholders::_1 ) ) ) );
      }

      void add_socket( Socket::pointer&& socket )
      {
        m_poll_descriptors.emplace_back( pollfd{ socket->fd, POLLIN, 0 } );
        m_sockets.emplace( std::make_pair( socket->fd, std::move( socket ) ) );
      }

      void start()
      {
        while ( true )
        {
          const int no_timeout( 0 );
          if ( poll( &m_poll_descriptors[0], m_poll_descriptors.size(), no_timeout ) < 0 )
          {
            //todo: handle network error
            return;
          }

          for ( auto& poll_descriptor : m_poll_descriptors )
          {
            const bool no_event( !( poll_descriptor.revents & POLLIN) );
            if ( no_event )
            {
              continue;
            }
            m_sockets[ poll_descriptor.fd ]->handle_event();
          }

        }
      }

    private:

      std::vector<pollfd> m_poll_descriptors;
      std::unordered_map<int, Socket::pointer> m_sockets;
  };
}

