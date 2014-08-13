#include <yarrr/hud.hpp>
#include <yarrr/physical_parameters.hpp>

namespace yarrr
{


Hud::Hud( GraphicalEngine& graphical_engine, PhysicalParameters& physical_parameters )
  : GraphicalObject( graphical_engine )
  , m_physical_parameters( physical_parameters )
{
  (void)m_physical_parameters;
}

void
Hud::draw() const
{
  print_lines( build_hud_lines() );
}


Hud::Lines
Hud::build_hud_lines() const
{
  Lines lines;
  lines.push_back( "coordinate" );
  lines.push_back( std::to_string( m_physical_parameters.coordinate.x ) );
  lines.push_back( std::to_string( m_physical_parameters.coordinate.y ) );
  lines.push_back( "velocity" );
  lines.push_back( std::to_string( m_physical_parameters.velocity.x ) );
  lines.push_back( std::to_string( m_physical_parameters.velocity.y ) );
  lines.push_back( "integrity" );
  lines.push_back( std::to_string( m_physical_parameters.integrity ) );
  return lines;
}


void
Hud::print_lines( const Lines& lines ) const
{
  size_t y_coordinate_of_line{ 120u };
  for ( const auto& line: lines )
  {
    m_graphical_engine.print_text( 0, y_coordinate_of_line, line, Colour::white );
    y_coordinate_of_line += GraphicalEngine::font_height;
  }
}

}

