#pragma once

#include <yarrr/types.hpp>
#include <thectci/id.hpp>

namespace yarrr
{
class Object;
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

    const static Colour White;
    const static Colour Green;
    const static Colour Red;
    const static Colour Strange;
};

inline bool operator==( const Colour& l, const Colour& r )
{
  return
    l.red == r.red &&
    l.green == r.green &&
    l.blue == r.blue &&
    l.alpha == r.alpha;
}

class Size
{
  public:
    int width;
    int height;
};


class GraphicalEngine
{
  public:
    add_polymorphic_ctci( "yarrr_graphical_engine" );
    virtual void draw_line( int x1, int y1, int x2, int y2, const Colour& ) = 0;
    virtual void draw_rectangle( int x1, int y1, int x2, int y2, const Colour& ) = 0;
    virtual void draw_particle( const PhysicalParameters&, uint64_t age ) = 0;
    virtual void draw_object_with_shape( const Object& ) = 0;
    virtual void draw_laser( const Object& ) = 0;
    virtual Size size_of_text( const std::string& text ) = 0;
    virtual void print_text( uint16_t x, uint16_t y, const std::string&, const Colour& ) = 0;
    virtual void focus_to( const Object& ) = 0;
    virtual ~GraphicalEngine() = default;
    virtual void update_screen() = 0;
    virtual const Coordinate& screen_resolution() const = 0;

    void draw_objects() const;

    void register_object( const GraphicalObject& );
    void delete_object( const GraphicalObject& );

  private:
    std::vector< std::reference_wrapper< const GraphicalObject > > m_objects;
};

}

