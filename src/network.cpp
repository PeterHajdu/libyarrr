#include <yarrr/network.hpp>
#include <unistd.h>

namespace yarrr
{
  Socket::Socket( int fd )
    : fd( fd )
  {
  }

  Socket::~Socket()
  {
    close( fd );
  }
}

