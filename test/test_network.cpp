
#include <igloo/igloo_alt.h>
#include <yarrr/network.hpp>

using namespace igloo;

namespace
{
  class DummyMessageFactory
  {};
}

Describe(a_socket_pool)
{
  It( is_instantiable )
  {
    yarrr::SocketPool< DummyMessageFactory > pool( 2000 );
  }
};

