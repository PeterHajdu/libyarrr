#pragma once
//todo: remove iostream and inet.h
#include <iostream>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#include <vector>
#include <poll.h>
#include <algorithm>


namespace yarrr
{
  class Socket
  {
    public:
      Socket( int fd );
      ~Socket();
      const int fd;
  };

  template < typename MessageFactory >
  class SocketPool
  {
    public:
      //todo: should be a default constructor
      //listen should register listening sockets
      SocketPool( int port )
        : m_listeningSocket(  socket( AF_INET, SOCK_STREAM, 0 ) )
        , m_port( port )
      {
      }

      bool listen()
      {
        struct sockaddr_in address;
        memset( &address, 0, sizeof( address ) );
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( m_port );

        if ( bind( m_listeningSocket.fd, (struct sockaddr *) &address, sizeof( address ) ) < 0 )
        {
          return false;
        }

        const int queue_length( 5 );
        if ( listen( m_listeningSocket.fd, queue_length ) < 0 )
        {
          return false;
        }
      }

      void start()
      {
        m_fds.emplace_back(pollfd{ m_listeningSocket.fd, POLLIN, 0 });
        while ( true )
        {
          const int no_timeout( 0 );
          if ( poll( &m_fds[0], m_fds.size(), no_timeout ) < 0 )
          {
            //todo: handle network error
            return;
          }

          handle_incoming_connection();
          handle_incoming_user_data();
        }
      }

    private:
      bool has_connections()
      {
        return m_fds.size() > 1;
      }

      void handle_incoming_user_data()
      {
        if ( !has_connections() )
        {
          return;
        }

        auto second_element( begin( m_fds ) + 1 );
        std::for_each( second_element, end( m_fds ),
            []( pollfd& poll_descriptor )
            {
              if ( poll_descriptor.revents == 0 )
              {
                return;
              }

              MessageFactory::received_data();
            } );
      }

      void handle_incoming_connection()
      {
        if ( m_fds[0].revents == 0 )
        {
          return;
        }

        struct sockaddr_in address;
        socklen_t sin_size = sizeof( address );

        m_fds.emplace_back(pollfd{
            accept(m_listeningSocket.fd, (struct sockaddr *) &address, &sin_size),
            POLLIN,
            0 });

        //todo: remove stupid printout
        std::cout << "connection from: " << inet_ntoa( address.sin_addr ) << std::endl;
      }

      std::vector<pollfd> m_fds;
      const Socket m_listeningSocket;
      const int m_port;
  };
}

