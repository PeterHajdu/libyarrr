#include <yarrr/bitmagic.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_serializer)
{
  void SetUp()
  {
    test_buffer.clear();
    test_serializer.reset( new yarrr::Serializer( test_buffer ) );
  }

  template < typename T >
  void compare_values_at_position( const T& value, size_t position )
  {
    const T& value_in_buffer( *reinterpret_cast< const T* >( &test_buffer[ position ] ) );
    AssertThat( value, Equals( value_in_buffer ) );
  }

  template < typename T >
  void check_single_value( const T& value )
  {
    test_buffer.clear();
    test_serializer->push_back( value );
    AssertThat( test_buffer, HasLength( sizeof( value ) ) );
    compare_values_at_position( value, 0 );
  }

  It( can_serialize_integral_types )
  {
    check_single_value( unsigned_64 );
    check_single_value( signed_8 );
  }

  It( appends_serialized_values )
  {
    test_serializer->push_back( signed_8 );
    test_serializer->push_back( unsigned_64 );

    AssertThat( test_buffer, HasLength( sizeof( signed_8 ) + sizeof( unsigned_64 ) ) );
    compare_values_at_position( signed_8, 0 );
    compare_values_at_position( unsigned_64, sizeof( signed_8 ) );
  }

  It( can_serialize_strings )
  {
    test_serializer->push_back( first_string );
    const std::string value_in_buffer( begin( test_buffer ), end( test_buffer ) );
    AssertThat( value_in_buffer, Equals( first_string ) );
  }

  const std::string first_string{ "alma" };
  const std::string second_string{ "fa" };
  const uint64_t unsigned_64{ 1123098348 };
  const int8_t signed_8{ 13 };
  typedef yarrr::Data TestBuffer;
  TestBuffer test_buffer;
  std::unique_ptr< yarrr::Serializer > test_serializer;
};

