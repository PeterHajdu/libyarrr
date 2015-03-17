#pragma once

#include <yarrr/graphical_engine.hpp>
#include <yarrr/physical_parameters.hpp>
#include <yarrr/basic_behaviors.hpp>
#include <numeric>

namespace test
{

class GraphicalEngine : public yarrr::GraphicalEngine
{
  public:
    class LineData
    {
      public:
        int x1;
        int y1;
        int x2;
        int y2;
        yarrr::Colour colour;
    };

    LineData last_line_data;
    virtual void draw_line( int x1, int y1, int x2, int y2, const yarrr::Colour& colour ) override
    {
      last_line_data = { x1, y1, x2, y2, colour };
    }

    LineData last_rectangle_data;
    virtual void draw_rectangle( int x1, int y1, int x2, int y2, const yarrr::Colour& colour ) override
    {
      last_rectangle_data = { x1, y1, x2, y2, colour };
    }

    const yarrr::Object* last_drawn_object_with_shape{ nullptr };
    virtual void draw_object_with_shape( const yarrr::Object& object ) override
    {
      last_drawn_object_with_shape = &object;
    }

    const yarrr::Object* last_drawn_laser;
    virtual void draw_laser( const yarrr::Object& laser ) override
    {
      last_drawn_laser = &laser;
    }

    yarrr::PhysicalParameters last_drawn_particle;
    uint64_t age_of_last_drawn_particle;
    virtual void draw_particle( const yarrr::PhysicalParameters& physical_parameters, uint64_t age ) override
    {
      last_drawn_particle = physical_parameters;
      age_of_last_drawn_particle = age;
    }


    yarrr::Size text_size{ 1, 1 };
    virtual yarrr::Size size_of_text( const std::string& text ) override
    {
      return text_size;
    }

    std::vector< std::string > printed_texts;
    std::string last_printed_text;
    virtual void print_text(
        uint16_t x,
        uint16_t y,
        const std::string& text,
        const yarrr::Colour& ) override
    {
      last_printed_text = text;
      printed_texts.push_back( text );
    }

    bool was_printed( const std::string& text ) const
    {
      return std::any_of( std::begin( printed_texts ), std::end( printed_texts ),
          [ this, text ]( const std::string& line )
          {
            return line.find( text ) != std::string::npos;
          } );
    }

    const yarrr::Object* last_focused_to;
    virtual void focus_to( const yarrr::Object& object ) override
    {
      last_focused_to = &object;
    }

    virtual void update_screen() override {}

    yarrr::Coordinate resolution{ 1000, 500 };
    virtual const yarrr::Coordinate& screen_resolution() const override
    {
      return resolution;
    }
};

}

