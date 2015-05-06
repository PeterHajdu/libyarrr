#pragma once

#include <yarrr/object_behavior.hpp>
#include <yarrr/goods.hpp>
#include <vector>

namespace yarrr
{

class CargoSpace : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_cargo_space" );
    CargoSpace();
    CargoSpace( const Id& id );
    virtual Pointer clone() const override;

    void add_goods( Goods );

    using GoodsContainer = std::vector< Goods >;
    const GoodsContainer& goods() const;

  private:
    virtual void serialize_behavior( Serializer& serializer ) const override final;
    virtual void deserialize_behavior( Deserializer& deserializer ) override final;

    GoodsContainer m_goods;
};

}

