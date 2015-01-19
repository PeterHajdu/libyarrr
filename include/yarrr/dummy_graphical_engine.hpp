#pragma once

#include <yarrr/graphical_engine.hpp>
namespace yarrr
{

class DummyGraphicalEngine : public yarrr::GraphicalEngine
{
  public:
    virtual void draw_rectangle( int x1, int y1, int x2, int y2, const Colour& ) override {}
    virtual void draw_line( int x1, int y1, int x2, int y2, const Colour& ) override {}
    virtual void draw_particle( const PhysicalParameters&, uint64_t ) override {}
    virtual void draw_object_with_shape( const Object& ) override {}
    virtual void draw_laser( const Object& ) override {}
    virtual Size size_of_text( const std::string& text ) override { return { 0, 0 }; }
    virtual void print_text( uint16_t, uint16_t, const std::string&, const yarrr::Colour& ) override {}
    virtual void print_text_tokens( uint16_t, uint16_t, const yarrr::TextTokens& ) override {}
    virtual void focus_to( const Object& ) override {}
    virtual void update_screen() override {}
    virtual const Coordinate& screen_resolution() const override { return resolution; }

  private:
    const Coordinate resolution{ 0, 0 };
};

}

