#pragma once

#include <yarrr/types.hpp>
#include <string>

namespace yarrr
{

class Serializer
{
  public:
    Serializer( Data& buffer )
      : m_buffer( buffer )
    {
    }

    template <typename T>
    void push_back( const T& value );

  private:
    Data& m_buffer;
};

template <typename T>
void Serializer::push_back( const T& value )
{
  const char* const start_of_value( reinterpret_cast< const char* >( &value ) );
  m_buffer.insert(
      std::end( m_buffer ),
      start_of_value,
      start_of_value + sizeof( value ) );
}

template <>
void Serializer::push_back< std::string >( const std::string& value )
{
  m_buffer.insert( std::end( m_buffer ), begin( value ), end( value ) );
}

}

