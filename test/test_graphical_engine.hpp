#pragma once

#include <yarrr/graphical_engine.hpp>

namespace test
{

class GraphicalEngine : public yarrr::GraphicalEngine
{
  public:
    virtual void draw_ship( const yarrr::PhysicalParameters& ) override {}

    std::vector< std::string > printed_texts;
    std::string last_printed_text;
    int x_of_printed_text{ 0 };
    int y_of_printed_text{ 0 };
    virtual void print_text(
        uint16_t x,
        uint16_t y,
        const std::string& text,
        const yarrr::Colour& )
    {
      last_printed_text = text;
      x_of_printed_text = x;
      y_of_printed_text = y;
      printed_texts.push_back( text );
    }

    virtual void focus_to( const yarrr::Coordinate& ) override {}
    virtual void update_screen() override {}
};

}

