#include <yarrr/physical_parameters.hpp>
#include "test_graphical_engine.hpp"
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

