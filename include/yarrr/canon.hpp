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
    int generate_next_index();
    void handle_command( const Command& ) const;
    void register_component_to( the::ctci::ComponentRegistry& components );
    PhysicalParameters generate_physical_parameters() const;

    PhysicalParameters* m_physical_parameters;
    int m_index;
    int m_number_of_canons;
};

}

