#pragma once

#include <yarrr/graphical_engine.hpp>
namespace yarrr
{

class DummyGraphicalEngine : public yarrr::GraphicalEngine
{
  public:
    virtual void draw_particle( const PhysicalParameters&, uint64_t ) override {}
    virtual void draw_ship( const yarrr::PhysicalParameters& ) override {}
    virtual void draw_laser( const yarrr::PhysicalParameters& ) override {}
    virtual void draw_loot( const PhysicalParameters& ) override {}
    virtual void print_text( uint16_t, uint16_t, const std::string&, const yarrr::Colour& ) {}
    virtual void print_text_tokens( uint16_t, uint16_t, const yarrr::TextTokens& ) override {}
    virtual void focus_to( const yarrr::Coordinate& ) override {}
    virtual void update_screen() override {}
};

}

