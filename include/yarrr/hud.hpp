#pragma once

#include <yarrr/graphical_engine.hpp>

namespace yarrr
{
class PhysicalParameters;

class Hud : public GraphicalObject
{
  public:
    Hud( GraphicalEngine&, PhysicalParameters& );
    virtual void draw() const override;
    virtual ~Hud() = default;
  private:
    typedef std::vector< std::string > Lines;
    Lines build_hud_lines() const;
    void print_lines( const Lines& ) const;
    PhysicalParameters& m_physical_parameters;
};

}

