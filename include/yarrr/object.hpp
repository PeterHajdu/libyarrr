#pragma once

#include <yarrr/entity.hpp>

#include <thectci/dispatcher.hpp>
#include <thectci/dispatcher.hpp>
#include <thectci/component_registry.hpp>

#include <memory>
#include <vector>

namespace yarrr
{

class ObjectBehavior
{
  public:
    typedef std::unique_ptr<ObjectBehavior> Pointer;
    virtual ~ObjectBehavior() = default;

    virtual void register_to( the::ctci::Dispatcher&, the::ctci::ComponentRegistry& ) = 0;
};

class ObjectUpdate;

class Object final : public the::ctci::Dispatcher
{
  public:
    typedef uint64_t Id;
    typedef std::unique_ptr<Object> Pointer;

    Object();
    Object( const Id& id );
    const Id id;

    void add_behavior( ObjectBehavior::Pointer&& behavior );

    ObjectUpdate generate_update() const;

  private:
    the::ctci::ComponentRegistry m_components;
    std::vector< ObjectBehavior::Pointer > m_behaviors;
};

class ObjectUpdate : public Entity
{
  public:
    add_polymorphic_ctci( "yarrr_object_update" );
    ObjectUpdate( const Object::Id& );
    const Object::Id id;

  private:
    virtual void do_serialize( Serializer& serializer ) const;
    virtual void do_deserialize( Deserializer& deserializer );
};

}

