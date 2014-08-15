#pragma once

#include <yarrr/object.hpp>
#include <thectci/id.hpp>

namespace yarrr
{

class Command;
class ShipControl;
class PhysicalParameters;

class Engine : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_engine" );

    Engine();
    virtual ~Engine();

    virtual void register_to( Object& ) override;

    virtual Pointer clone() const override;

  private:
    void handle_command( const yarrr::Command& ) const;
    std::unique_ptr< ShipControl > m_ship_control;
    PhysicalParameters* m_physical_parameters;
};

}

