#include <yarrr/cli.hpp>
#include <yarrr/chat_message.hpp>

const std::string yarrr::Cli::m_prompt{ "$ " };

namespace yarrr
{

Cli::Cli( int x, int y, GraphicalEngine& graphical_engine )
  : GraphicalObject( graphical_engine )
  , m_x( x )
  , m_y( y )
{
}


void
Cli::draw() const
{
  m_graphical_engine.print_text(
      m_x, m_y,
      m_prompt + m_text,
      { 255, 255, 255, 255 } );
}


void
Cli::append( const std::string& text )
{
  m_text += text;
}


void
Cli::backspace()
{
  m_text.pop_back();
}


void
Cli::finalize()
{
  if ( m_text.empty() )
  {
    return;
  }

  dispatch( ChatMessage( m_text ) );
  m_text.clear();
}

}

