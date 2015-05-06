#include <yarrr/cargo.hpp>
#include <yarrr/entity_factory.hpp>
#include <yarrr/bitmagic.hpp>
#include <iostream>

namespace
{

yarrr::AutoEntityRegister< yarrr::CargoSpace > auto_cargo_space_register;
using size_type = uint8_t;

}

namespace yarrr
{

CargoSpace::CargoSpace()
  : ObjectBehavior( rarely_synchronize() )
{
}


CargoSpace::CargoSpace( const Id& id )
  : ObjectBehavior( rarely_synchronize(), id )
{
}


ObjectBehavior::Pointer
CargoSpace::clone() const
{
  auto a_clone( std::make_unique< CargoSpace >( id() ) );

  for ( const auto& goods : m_goods )
  {
    a_clone->add_goods( goods );
  }

  return std::move( a_clone );
}


void
CargoSpace::add_goods( Goods goods )
{
  m_goods.emplace_back( std::move( goods ) );
  force_synchronization();
}


const CargoSpace::GoodsContainer&
CargoSpace::goods() const
{
  return m_goods;
}


void
CargoSpace::serialize_behavior( Serializer& serializer ) const
{
  serializer.push_back< size_type >( m_goods.size() );

  for ( const auto& goods : m_goods )
  {
    serializer.push_back<std::string>( goods.name );
    serializer.push_back<std::string>( goods.type );
    serializer.push_back<Goods::amount_type>( goods.amount );
  }
}


void
CargoSpace::deserialize_behavior( Deserializer& deserializer )
{
  const size_type number_of_goods( deserializer.pop_front< size_type >() );
  for ( auto i( 0u ); i < number_of_goods; ++i )
  {
    auto name( deserializer.pop_front<std::string>() );
    auto type( deserializer.pop_front<std::string>() );
    auto amount( deserializer.pop_front<Goods::amount_type>() );
    add_goods( Goods{
        std::move( name ),
        std::move( type ),
        std::move( amount ) } );
  }
}


}

