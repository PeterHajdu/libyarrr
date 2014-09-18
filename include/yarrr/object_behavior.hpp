#pragma once

#include <yarrr/entity.hpp>
#include <vector>

namespace yarrr
{

constexpr inline int synchronize_nth( int i )
{
  return i;
}

constexpr inline int rarely_synchronize()
{
  return 25;
}

constexpr inline int always_synchronize()
{
  return 1;
}

constexpr inline int do_not_synchronize()
{
  return 0;
}

class Object;

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

}

