#include <yarrr/terminal.hpp>
#include <yarrr/chat_message.hpp>
#include <algorithm>
#include <thectci/hash.hpp>

namespace
{
  yarrr::Colour colorize( const std::string& text )
  {
    uint32_t hash( the::ctci::hash( text.c_str() ) );
    return {
      static_cast<uint8_t>( hash ),
      static_cast<uint8_t>( hash >> 8 ),
      static_cast<uint8_t>( hash >> 16 ),
      255u };
  }
}

namespace yarrr
{

Terminal::Terminal(
    GraphicalEngine& engine,
    size_t number_of_messages )
  : GraphicalObject( engine )
  , m_number_of_shown_messages( number_of_messages )
{
  register_listener< yarrr::ChatMessage >(
      std::bind( &Terminal::handle_chat_message, this, std::placeholders::_1 ) );
}

void
Terminal::handle_chat_message( const ChatMessage& message )
{
  m_messages.push_back( {
        { message.sender() + ": ", colorize( message.sender() ) },
        { message.message(), { 255, 255, 255, 255 } } } );
}

void
Terminal::draw() const
{
  //todo: this should be retrieved somehow from graphical engine
  const size_t line_height( 15 );
  const size_t length( std::min( size_t( m_messages.size() ), m_number_of_shown_messages ) );

  const size_t start_index( m_messages.size() - length );
  for ( size_t i( 0 ); i < length; ++i )
  {
    m_graphical_engine.print_text_tokens(
        0, i * line_height,
        m_messages[ start_index + i ] );
  }
}

}

