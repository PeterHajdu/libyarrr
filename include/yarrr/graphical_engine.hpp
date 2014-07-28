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

class GraphicalEngine
{
  public:
    add_polymorphic_ctci( "yarrr_graphical_engine" );
    virtual void draw_ship( const PhysicalParameters& ) = 0;
    virtual void focus_to( const yarrr::Coordinate& ) = 0;
    virtual ~GraphicalEngine() = default;

    void draw_objects() const;

    void register_object( const GraphicalObject& );
    void delete_object( const GraphicalObject& );

  private:
    std::vector< std::reference_wrapper< const GraphicalObject > > m_objects;
};

}

