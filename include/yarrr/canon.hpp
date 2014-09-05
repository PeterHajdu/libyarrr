#pragma once

#include <yarrr/object.hpp>

namespace yarrr
{

class Command;
class PhysicalParameters;
class Canon : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_canon" );
    Canon();

    virtual void register_to( Object& ) override;
    virtual Pointer clone() const override;

  private:
    void handle_command( const Command& ) const;
    PhysicalParameters* m_physical_parameters;
};

}

