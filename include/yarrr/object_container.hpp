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

    bool has_object_with_id( Object::Id ) const;
    const yarrr::Object& object_with_id( Object::Id ) const;

  private:
    typedef std::unordered_map< Object::Id, Object::Pointer > Objects;
    Objects m_objects;
};

}

