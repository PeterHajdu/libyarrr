#pragma once

#include <yarrr/object.hpp>

namespace yarrr
{

class Item : public ObjectBehavior
{
  public:
    Item( const std::string& name );
    Item( const Id& id, const std::string& name );
    virtual ~Item() = default;
    const std::string& name() const;

  private:
    virtual void do_register_to( Object& ) override final;
    virtual void register_item_to( Object& ) {};

    const std::string m_name;
};

}

