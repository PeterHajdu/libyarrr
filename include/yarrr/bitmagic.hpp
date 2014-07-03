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

template <> inline
void Serializer::push_back< std::string >( const std::string& value )
{
  push_back( static_cast<uint32_t>( value.size() ) );
  m_buffer.insert( std::end( m_buffer ), begin( value ), end( value ) );
}

class Deserializer
{
  public:
    Deserializer( const Data& buffer )
      : m_position( &buffer[0] )
      , m_end_of_buffer( m_position + buffer.size() )
    {
    }

    template <typename T>
    T pop_front();

    size_t bytes_left() const
    {
      return m_end_of_buffer - m_position;
    }

  private:
    const char* m_position;
    const char* m_end_of_buffer;
};

template <typename T>
T Deserializer::pop_front()
{
  const char * const position( m_position );
  m_position += sizeof( T );
  return *reinterpret_cast< const T* >( position );
}

template <> inline
std::string Deserializer::pop_front<std::string>()
{
  const uint32_t length( pop_front< uint32_t >() );
  const char * const position( m_position );
  m_position += length;
  return std::string( position, length );
}

}

