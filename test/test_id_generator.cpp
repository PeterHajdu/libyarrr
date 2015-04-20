#include <yarrr/id_generator.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe( an_id_generator )
{

  void SetUp()
  {
    id_generator = std::make_unique< yarrr::IdGenerator >();
  }

  It ( generates_distinct_ids_for_a_given_key )
  {
    const std::string a_key{ "a_key" };
    AssertThat( id_generator->generate( a_key ), !Equals( id_generator->generate( a_key ) ) );
  }

  std::unique_ptr< yarrr::IdGenerator > id_generator;

};

