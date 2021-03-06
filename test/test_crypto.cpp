#include <yarrr/crypto.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( the_authentication_hash )
{
  It ( is_sha_256 )
  {
    AssertThat(
        yarrr::auth_hash( "dogfood" ),
        Equals( "e674f78d2bf55fd93c878f7fe14448abe677e7c3160f820ac6a012e457520b81" ) );
  }

};

Describe( the_random_number_generator )
{
  It ( has_one )
  {
    const std::string random_data( yarrr::random( 10 ) );
  }

  It ( can_be_hashed_directly )
  {
    for ( int i( 0 ); i < 10; ++i )
    {
      std::cout << yarrr::auth_hash( yarrr::random( 512 ) ) << std::endl;
    }
  }
};

