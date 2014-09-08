#pragma once

#include <yarrr/entity.hpp>

#include <thectci/dispatcher.hpp>
#include <thectci/id.hpp>
#include <thectci/component_registry.hpp>

#include <memory>
#include <vector>
#include <functional>

namespace yarrr
{

class Object;

class ObjectBehavior : public Entity
{
  public:
    add_pure_polymorphic_ctci();
    typedef std::unique_ptr<ObjectBehavior> Pointer;

    enum ShouldSynchronize : bool
    {
      synchronize = true,
      do_not_syncronize = false
    };

    ObjectBehavior( ShouldSynchronize );
    const ShouldSynchronize should_synchronize;
    void register_to( Object& );

    virtual ~ObjectBehavior() = default;
    virtual Pointer clone() const = 0;

  protected:
    Object* m_object;

  private:
    virtual void do_register_to( Object& ) = 0;
};

typedef std::vector< ObjectBehavior::Pointer > BehaviorContainer;

class ObjectUpdate;
class Object final
{
  public:
    typedef uint64_t Id;
    typedef std::unique_ptr<Object> Pointer;

    Object();
    Object( const Id& id );
    const Id id;
    the::ctci::Dispatcher dispatcher;
    the::ctci::ComponentRegistry components;

    void add_behavior( ObjectBehavior::Pointer&& behavior );
    std::unique_ptr< ObjectUpdate > generate_update() const;

  private:
    BehaviorContainer m_behaviors;
};

class ObjectUpdate : public Entity
{
  public:
    typedef std::unique_ptr< ObjectUpdate > Pointer;

    add_polymorphic_ctci( "yarrr_object_update" );
    ObjectUpdate();
    ObjectUpdate( const Object::Id&, BehaviorContainer&& );
    const Object::Id& id() const;
    Object::Pointer create_object() const;
    void update_object( const Object& ) const;

  private:
    Object::Id m_id;

    virtual void do_serialize( Serializer& serializer ) const;
    virtual void do_deserialize( Deserializer& deserializer );

    BehaviorContainer m_behaviors;
};

}

