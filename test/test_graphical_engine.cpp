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

  It( can_draw_ships )
  {
    yarrr::PhysicalParameters ship_parameters;
    test_engine->draw_ship( ship_parameters );
  }

  It( can_focus_to_a_coordinate )
  {
    test_engine->focus_to( { 100, 100 } );
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

