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
      typedef std::unique_ptr< Socket > Pointer;
      Socket( int fd );
      virtual ~Socket();

      const int fd;
      virtual void handle_event() = 0;
  };

  class SocketPool
  {
    public:
      typedef std::function<void(Socket&)> Callback;
      SocketPool( Callback new_socket, Callback read_data );
      void listen( int port );

      //todo: implement address parsing class
      bool connect( const std::string& address, int port );
      void start();

    private:
      void add_socket_with_callback( Socket::Pointer&& socket );
      void add_socket( Socket::Pointer&& socket );
      std::vector<pollfd> m_poll_descriptors;
      std::unordered_map<int, Socket::Pointer> m_sockets;

      Callback m_new_socket_callback;
      Callback m_read_data_callback;
  };
}

