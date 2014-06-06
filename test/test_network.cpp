
#include <igloo/igloo_alt.h>
#include <yarrr/network.hpp>

using namespace igloo;

namespace
{
  void new_socket_callback( yarrr::Socket& )
  {
  }

  void read_data_callback( yarrr::Socket& )
  {
  }
}

Describe(a_socket_pool)
{
  It( is_instantiable )
  {
    yarrr::SocketPool pool( &new_socket_callback, &read_data_callback );
  }
};

