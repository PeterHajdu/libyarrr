#include <yarrr/network.hpp>
#include <unistd.h>

namespace yarrr
{
  Socket::Socket( int fd )
    : fd( fd )
  {
  }

  void
  Socket::allow_address_reuse() const
  {
    const int true_value( 1 );
    setsockopt(
        fd,
        SOL_SOCKET,
        SO_REUSEADDR,
        &true_value,
        sizeof( true_value ) );
  }

  Socket::~Socket()
  {
    close( fd );
  }
}

