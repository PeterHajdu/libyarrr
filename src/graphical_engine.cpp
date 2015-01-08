#include <yarrr/graphical_engine.hpp>
#include <algorithm>
#include <functional>

namespace yarrr
{

const Colour Colour::White{ 255, 255, 255, 255 };
const Colour Colour::Red{ 255, 0, 0, 255 };
const Colour Colour::Green{ 0, 255, 0, 255 };
const Colour Colour::Strange{ 255, 255, 0, 255 };
size_t GraphicalEngine::font_height{ 15 };
size_t GraphicalEngine::font_width{ 10 };

GraphicalObject::GraphicalObject( GraphicalEngine& graphical_engine )
  : m_graphical_engine( graphical_engine )
{
  m_graphical_engine.register_object( *this );
}


GraphicalObject::~GraphicalObject()
{
  m_graphical_engine.delete_object( *this );
}


void
GraphicalEngine::register_object( const GraphicalObject& object )
{
  m_objects.emplace_back( object );
}


void
GraphicalEngine::delete_object( const GraphicalObject& object )
{
  m_objects.erase(
      std::remove_if( std::begin( m_objects ), std::end( m_objects ),
        [ &object ]( const std::reference_wrapper< const GraphicalObject >& object_in_container )
        {
          return &object == &object_in_container.get();
        } ),
      std::end( m_objects ) );
}


void
GraphicalEngine::draw_objects() const
{
  for ( const auto& object : m_objects )
  {
    object.get().draw();
  }
}

int
TextToken::width() const
{
  return text.length() * GraphicalEngine::font_width;
}

}

