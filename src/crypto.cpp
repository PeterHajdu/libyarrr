#include <yarrr/crypto.hpp>
#include <yarrr/log.hpp>
#include "sha256.hpp"
#include <fstream>
#include <cassert>

namespace yarrr
{

std::string
auth_hash( const std::string& text )
{
  thelog( log::debug )( "Generating auth hash." );
  return sha256( text );
}


std::string
random( size_t len )
{
  thelog( log::debug )( "Generating random data of length.", len );
  std::fstream random_stream( "/dev/urandom", std::ios::in | std::ios::binary );
  std::string random_data( len, 0 );
  random_stream.read( &random_data[0], len );

  if ( !random_stream.good() )
  {
    assert( "Failed to generate random data.  Please give read access to /dev/urandom." == nullptr );
  }

  return random_data;
}

}

