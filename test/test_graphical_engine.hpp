#pragma once

#include <yarrr/graphical_engine.hpp>

namespace test
{

class GraphicalEngine : public yarrr::GraphicalEngine
{
  public:
    virtual void draw_ship( const yarrr::PhysicalParameters& ) override {}
    virtual void print_text( uint16_t x, uint16_t y, const std::string&, const yarrr::Colour& ) {}
    virtual void focus_to( const yarrr::Coordinate& ) override {}
    virtual void update_screen() override {}
};

}

