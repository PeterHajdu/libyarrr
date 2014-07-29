#include <yarrr/terminal.hpp>
#include <yarrr/chat_message.hpp>
#include <thectci/dispatcher.hpp>
#include <algorithm>

namespace yarrr
{

Terminal::Terminal(
    GraphicalEngine& engine,
    the::ctci::Dispatcher& dispatcher,
    int number_of_messages )
  : GraphicalObject( engine )
  , m_number_of_shown_messages( number_of_messages )
{
  dispatcher.register_listener< yarrr::ChatMessage >(
      std::bind( &Terminal::handle_chat_message, this, std::placeholders::_1 ) );
}

void
Terminal::handle_chat_message( const ChatMessage& message )
{
  m_messages.push_back( message.message() );
}

void
Terminal::draw() const
{
  const size_t line_height( 12 );
  const int length( std::min( int( m_messages.size() ), m_number_of_shown_messages ) );
  for ( int i( m_messages.size() - length ); i < m_messages.size(); ++i )
  {
    m_graphical_engine.print_text( 0, i * line_height, m_messages[ i ].c_str(), { 255, 255, 255, 255 } );
  }
}

}

