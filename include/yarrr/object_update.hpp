#pragma once

#include <yarrr/object.hpp>
#include <yarrr/object_behavior.hpp>

namespace yarrr
{

class ObjectUpdate : public Entity
{
  public:
    typedef std::unique_ptr< ObjectUpdate > Pointer;

    ObjectUpdate();
    ObjectUpdate( const Object::Id&, BehaviorContainer&& );
    const Object::Id& id() const;

    void update_object( Object& ) const;
    virtual Object::Pointer create_object() const;

  private:
    Object::Id m_id;

    virtual void do_serialize( Serializer& serializer ) const;
    virtual void do_deserialize( Deserializer& deserializer );

    BehaviorContainer m_behaviors;
};

class BasicObjectUpdate : public ObjectUpdate
{
  public:
    add_polymorphic_ctci( "yarrr_object_update" );
    BasicObjectUpdate() = default;
    BasicObjectUpdate( const Object::Id&, BehaviorContainer&& );
};

class ObjectInitializer : public ObjectUpdate
{
  public:
    add_polymorphic_ctci( "yarrr_object_initializer" );
    ObjectInitializer() = default;
    ObjectInitializer( const Object::Id&, BehaviorContainer&& );
};

}

