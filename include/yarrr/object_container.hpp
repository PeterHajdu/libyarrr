#pragma once

#include <yarrr/object.hpp>
#include <thectci/dispatcher.hpp>

namespace yarrr
{

class ObjectContainer : public the::ctci::Dispatcher
{
  public:
    void add_object( Object::Id, Object::Pointer&& );
    void delete_object( Object::Id );

  private:
    typedef std::unordered_map< Object::Id, Object::Pointer > Objects;
    Objects m_objects;
};

}

