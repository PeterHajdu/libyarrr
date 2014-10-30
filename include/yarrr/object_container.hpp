#pragma once

#include <yarrr/object.hpp>
#include <yarrr/object_update.hpp>
#include <thectci/dispatcher.hpp>

namespace yarrr
{

class ObjectContainer : public the::ctci::Dispatcher
{
  public:
    typedef size_t size_type;
    void add_object( Object::Pointer&& );
    void delete_object( Object::Id );

    bool has_object_with_id( Object::Id ) const;
    Object& object_with_id( Object::Id ) const;

    size_type size() const;

    std::vector< ObjectUpdate::Pointer > generate_object_updates() const;
    void handle_object_update( const ObjectUpdate& );

    void check_collision() const;

    typedef std::unordered_map< Object::Id, Object::Pointer > Objects;
    const Objects& objects() const;

  private:
    Objects m_objects;
};

}

