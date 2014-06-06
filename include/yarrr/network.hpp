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

  class SocketPool
  {
    public:
      void listen( int port );
      void add_socket( Socket::pointer&& socket );
      void start();

    private:
      std::vector<pollfd> m_poll_descriptors;
      std::unordered_map<int, Socket::pointer> m_sockets;

  };
}

