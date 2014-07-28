#pragma once

#include <yarrr/graphical_engine.hpp>
#include <thectci/multiplexer.hpp>

namespace yarrr
{

class Cli :
  public GraphicalObject,
  public the::ctci::Multiplexer
{
  public:
    Cli( int x, int y, GraphicalEngine& );
    virtual void draw() const override;

    void append( const std::string& );
    void finalize();

  private:
    std::string m_prompt;
    const int m_x;
    const int m_y;
};

}

