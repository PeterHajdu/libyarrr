#pragma once

#include <yarrr/object_behavior.hpp>

#include <thectci/dispatcher.hpp>
#include <thectci/component_registry.hpp>
#include <thectci/id.hpp>

#include <memory>

namespace yarrr
{

class ObjectUpdate;
class Object final
{
  public:
    typedef uint64_t Id;
    typedef std::unique_ptr<Object> Pointer;

    Object();
    Object( const Id& id );
    Object( const Object& ) = delete;
    Object& operator=( const Object& ) = delete;

    static Pointer create();

    the::ctci::Dispatcher dispatcher;
    the::ctci::ComponentRegistry components;

    Pointer clone() const;

    Id id() const;

    void add_behavior( ObjectBehavior::Pointer&& behavior );
    void add_behavior_clone( ObjectBehavior& behavior );

    void update_behavior( ObjectBehavior::Pointer&& behavior );

    std::unique_ptr< ObjectUpdate > generate_update() const;

  private:
    void force_full_synchronization() const;

    BehaviorContainer m_behaviors;
    const Id m_id;

    static const int object_initialization_period;
    mutable int m_object_update_index;
};

template < typename Component >
bool has_component( const Object& object )
{
  return object.components.has_component< Component >();
}

template < typename Component >
Component& component_of( const Object& object )
{
  return object.components.component< Component >();
}

}

