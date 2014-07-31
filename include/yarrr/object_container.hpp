#pragma once

#include <yarrr/object.hpp>
#include <thectci/dispatcher.hpp>

namespace yarrr
{

class ObjectContainer : public the::ctci::Dispatcher
{
  public:
    typedef size_t size_type;
    void add_object( Object::Id, Object::Pointer&& );
    void delete_object( Object::Id );

    bool has_object_with_id( Object::Id ) const;
    const Object& object_with_id( Object::Id ) const;

    size_type size() const;

  private:
    typedef std::unordered_map< Object::Id, Object::Pointer > Objects;
    Objects m_objects;
};

}

