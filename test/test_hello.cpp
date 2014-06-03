#include <igloo/igloo_alt.h>

#include <yarrr/hello.hpp>

using namespace igloo;

Describe(hello)
{
    It(should_greet_me)
    {
      const std::string name( "peti" );
      yarrr::hello hello( name );
      AssertThat( hello.greet(), Equals( "hello " + name ) );
    }
};

