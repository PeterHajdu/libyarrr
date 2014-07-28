#pragma once

#include <yarrr/types.hpp>
#include <thectci/id.hpp>

namespace yarrr
{
class PhysicalParameters;

class GraphicalEngine;
class GraphicalObject
{
  public:
    GraphicalObject( GraphicalEngine& graphical_engine );
    virtual ~GraphicalObject();
    virtual void draw() const = 0;

  protected:
    GraphicalEngine& m_graphical_engine;
};

class Colour
{
  public:
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};

class GraphicalEngine
{
  public:
    add_polymorphic_ctci( "yarrr_graphical_engine" );
    virtual void draw_ship( const PhysicalParameters& ) = 0;
    virtual void print_text( uint16_t x, uint16_t y, const std::string&, const Colour& ) = 0;
    virtual void focus_to( const yarrr::Coordinate& ) = 0;
    virtual ~GraphicalEngine() = default;
    virtual void update_screen() = 0;

    void draw_objects() const;

    void register_object( const GraphicalObject& );
    void delete_object( const GraphicalObject& );

  private:
    std::vector< std::reference_wrapper< const GraphicalObject > > m_objects;
};

}

