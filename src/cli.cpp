#include <yarrr/cli.hpp>
#include <yarrr/chat_message.hpp>

namespace
{
  const std::string default_prompt{ "$ " };
}

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
  m_graphical_engine.print_text( m_x, m_y, default_prompt + m_prompt, { 255, 255, 255, 255 } );
}


void
Cli::append( const std::string& text )
{
  m_prompt += text;
}


void
Cli::backspace()
{
  m_prompt.pop_back();
}


void
Cli::finalize()
{
  dispatch( ChatMessage( m_prompt ) );
  m_prompt.clear();
}

}

