#include <yarrr/physical_parameters.hpp>
#include <yarrr/test_graphical_engine.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{
  class TestGraphicalObject : public yarrr::GraphicalObject
  {
    public:
      TestGraphicalObject( yarrr::GraphicalEngine& engine )
        : GraphicalObject( engine )
      {
      }

      ~TestGraphicalObject()
      {
      }

      mutable bool draw_has_been_called{ false };
      virtual void draw() const override
      {
        draw_has_been_called = true;
      }
  };
}

Describe(a_graphical_engine)
{
  void SetUp()
  {
    test_engine.reset( new test::GraphicalEngine() );
  }

  It( draws_registered_objects_when_screen_is_updated )
  {
    TestGraphicalObject test_object( *test_engine );
    test_engine->draw_objects();
    AssertThat( test_object.draw_has_been_called, Equals( true ) );
  }

  It( removes_deleted_objects )
  {
    {
      std::unique_ptr< TestGraphicalObject > test_object( new TestGraphicalObject( *test_engine ) );
      test_engine->draw_objects();
      AssertThat( test_object->draw_has_been_called, Equals( true ) );
    }
    test_engine->draw_objects();
  }

  std::unique_ptr< yarrr::GraphicalEngine > test_engine;
};

Describe( a_text_token )
{

  It( calculates_token_width )
  {
    AssertThat( text_token.width(), Equals( text_length ) );
  }

  It( has_a_constructor_with_text )
  {
    const std::string dogfood( "dogfood" );
    const yarrr::TextToken token( dogfood );
    AssertThat( token.text, Equals( dogfood ) );
  }

  It( has_a_constructor_with_text_and_colour )
  {
    const std::string dogfood( "dogfood" );
    const yarrr::TextToken token( dogfood, yarrr::Colour::White );
    AssertThat( token.text, Equals( dogfood ) );
    AssertThat( token.colour, Equals( yarrr::Colour::White ) );
  }

  const yarrr::TextToken text_token{ "dogfood", {} };
  const int text_length{ static_cast< int >( text_token.text.length() * yarrr::GraphicalEngine::font_width ) };
};

