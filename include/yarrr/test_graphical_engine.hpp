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

    const yarrr::Object* last_drawn_object_with_shape{ nullptr };
    virtual void draw_object_with_shape( const yarrr::Object& object ) override
    {
      last_drawn_object_with_shape = &object;
    }

    yarrr::PhysicalParameters last_drawn_laser;
    virtual void draw_laser( const yarrr::PhysicalParameters& parameters ) override
    {
      last_drawn_laser = parameters;
    }

    virtual void draw_loot( const yarrr::PhysicalParameters& parameters ) override {}

    yarrr::PhysicalParameters last_drawn_particle;
    uint64_t age_of_last_drawn_particle;
    virtual void draw_particle( const yarrr::PhysicalParameters& physical_parameters, uint64_t age ) override
    {
      last_drawn_particle = physical_parameters;
      age_of_last_drawn_particle = age;
    }

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

    virtual void print_text_tokens( uint16_t x, uint16_t y, const yarrr::TextTokens& tokens ) override
    {
      std::string accumulated_text;
      for ( const auto& token : tokens )
      {
        accumulated_text += token.text;
      }
      print_text( x, y, accumulated_text, { 255, 255, 255, 255 } );
    }

    yarrr::Coordinate last_focused_to;
    virtual void focus_to( const yarrr::Coordinate& coordinate ) override
    {
      last_focused_to = coordinate;
    }

    virtual void update_screen() override {}

    virtual const yarrr::Coordinate& screen_resolution() const
    {
      return resolution;
    }

  private:
    const yarrr::Coordinate resolution{ 0, 0 };
};

}

