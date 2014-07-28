#include <yarrr/graphical_engine.hpp>
#include <yarrr/physical_parameters.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{
  class TestGraphicalEngine : public yarrr::GraphicalEngine
  {
    public:
      virtual void draw_ship( const yarrr::PhysicalParameters& ) override {}
      virtual void print_text( uint16_t x, uint16_t y, const std::string& ) override {}
      virtual void focus_to( const yarrr::Coordinate& ) override {}
      virtual void update_screen() override {}
  };

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
    test_engine.reset( new TestGraphicalEngine );
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

