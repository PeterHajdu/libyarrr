#pragma once

#include <yarrr/types.hpp>
#include <string>
#include <cassert>

namespace yarrr
{

template < typename T >
T extract( const char* from )
{
  return *reinterpret_cast<const T*>( from );
}

class Serializer
{
  public:
    Serializer( Data& buffer )
      : m_buffer( buffer )
    {
    }

    template <typename T>
    Serializer& push_back( const T& value );

  private:
    Data& m_buffer;
};

template <typename T>
Serializer& Serializer::push_back( const T& value )
{
  const char* const start_of_value( reinterpret_cast< const char* >( &value ) );
  m_buffer.insert(
      std::end( m_buffer ),
      start_of_value,
      start_of_value + sizeof( value ) );
  return *this;
}

template <> inline
Serializer& Serializer::push_back< std::string >( const std::string& value )
{
  push_back( static_cast<uint32_t>( value.size() ) );
  m_buffer.insert( std::end( m_buffer ), begin( value ), end( value ) );
  return *this;
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

    template <typename T>
    T peek() const;

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
  assert( bytes_left() >= sizeof( T ) );
  const char * const position( m_position );
  m_position += sizeof( T );
  return *reinterpret_cast< const T* >( position );
}

template <> inline
std::string Deserializer::pop_front<std::string>()
{
  const uint32_t length( pop_front< uint32_t >() );
  assert( bytes_left() >= length );
  const char * const position( m_position );
  m_position += length;
  return std::string( position, length );
}

template <typename T>
T Deserializer::peek() const
{
  assert( bytes_left() >= sizeof( T ) );
  const char * const position( m_position );
  return *reinterpret_cast< const T* >( position );
}

template <> inline
std::string Deserializer::peek<std::string>() const
{
  typedef uint32_t LengthType;
  const LengthType length( peek< LengthType >() );
  assert( bytes_left() - sizeof( LengthType ) >= length );
  const char * const position( m_position + sizeof( LengthType ) );
  return std::string( position, length );
}

}

