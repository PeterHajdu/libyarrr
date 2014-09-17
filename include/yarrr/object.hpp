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

constexpr inline int synchronize_nth( int i )
{
  return i;
}

constexpr inline int rarely_synchronize()
{
  return 10;
}

constexpr inline int always_synchronize()
{
  return 1;
}

constexpr inline int do_not_synchronize()
{
  return 0;
}

class ObjectBehavior : public Entity
{
  public:
    add_pure_polymorphic_ctci();
    typedef std::unique_ptr<ObjectBehavior> Pointer;

    typedef uint64_t Id;

    ObjectBehavior( int synchronization_period );
    ObjectBehavior( int synchronization_period, const Id& id );

    const Id& id() const;

    void register_to( Object& );

    virtual ~ObjectBehavior() = default;
    virtual Pointer clone() const = 0;

    virtual void update( const ObjectBehavior& ) {}

    void force_synchronization();
    bool should_synchronize();

  protected:
    Id m_id;
    Object* m_object;

  private:
    virtual void do_register_to( Object& ) {}
    virtual void do_serialize( Serializer& serializer ) const override final;
    virtual void do_deserialize( Deserializer& deserializer ) override final;

    virtual void serialize_behavior( Serializer& serializer ) const {};
    virtual void deserialize_behavior( Deserializer& deserializer ) {};

    const int m_synchronization_period;
    int m_synchronization_index;
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
    void update_behavior( ObjectBehavior::Pointer&& behavior );
    void force_full_synchronization();

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
    void update_object( Object& ) const;

  private:
    Object::Id m_id;

    virtual void do_serialize( Serializer& serializer ) const;
    virtual void do_deserialize( Deserializer& deserializer );

    BehaviorContainer m_behaviors;
};

template < typename Component >
Component& component_of( const Object& object )
{
  return object.components.component< Component >();
}

}

