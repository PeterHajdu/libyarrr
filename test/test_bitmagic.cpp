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

  void check_string_at( const std::string& string, const char* buffer )
  {
    const uint32_t length( *reinterpret_cast<const uint32_t*>( buffer ) );
    AssertThat( length, Equals( string.size() ) );
    const char* const start_of_string( buffer + sizeof( length ) );
    AssertThat( std::string( start_of_string, start_of_string + length ), Equals( string ) );
  }

  It( can_serialize_strings )
  {
    test_serializer->push_back( first_string );
    test_serializer->push_back( second_string );

    check_string_at( first_string, &test_buffer[0] );
    const size_t length_of_length( 4 );
    check_string_at( second_string, &test_buffer[ length_of_length + first_string.size() ] );
  }

  const std::string first_string{ "alma" };
  const std::string second_string{ "fa" };
  const uint64_t unsigned_64{ 1123098348 };
  const int8_t signed_8{ 13 };
  typedef yarrr::Data TestBuffer;
  TestBuffer test_buffer;
  std::unique_ptr< yarrr::Serializer > test_serializer;
};

Describe(a_deserializer)
{
  void SetUp()
  {
    test_buffer.clear();
    test_serializer.reset( new yarrr::Serializer( test_buffer ) );
  }

  void set_up_deserializer()
  {
    test_deserializer.reset( new yarrr::Deserializer( test_buffer ) );
  }

  It( can_deserialize_serialized_integral_data )
  {
    test_serializer->push_back( signed_8 );
    test_serializer->push_back( unsigned_64 );
    set_up_deserializer();
    AssertThat( test_deserializer->pop_front< int8_t >(), Equals( signed_8 ) );
    AssertThat( test_deserializer->pop_front< uint64_t >(), Equals( unsigned_64 ) );
  }

  It( can_deserialize_strings )
  {
    test_serializer->push_back( first_string );
    test_serializer->push_back( second_string );
    set_up_deserializer();
    AssertThat( test_deserializer->pop_front< std::string >(), Equals( first_string ) );
    AssertThat( test_deserializer->pop_front< std::string >(), Equals( second_string ) );
  }

  const std::string first_string{ "alma" };
  const std::string second_string{ "fa" };
  const uint64_t unsigned_64{ 1123098348 };
  const int8_t signed_8{ 13 };
  typedef yarrr::Data TestBuffer;
  TestBuffer test_buffer;
  std::unique_ptr< yarrr::Serializer > test_serializer;
  std::unique_ptr< yarrr::Deserializer > test_deserializer;
};

