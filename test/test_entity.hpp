#pragma once

#include <yarrr/entity.hpp>
#include <yarrr/bitmagic.hpp>

namespace test
{

  class Entity : public yarrr::Entity
  {
    public:
      int data;

      Entity()
        : data( 0 )
      {
      }

      Entity( int data )
        : data( data )
      {
      }

      add_polymorphic_ctci( "yarrr_test_entity" );

    private:
      virtual void do_serialize( yarrr::Serializer& serializer ) const override
      {
        serializer.push_back( data );
      }

      virtual void do_deserialize( yarrr::Deserializer& deserializer ) override
      {
        data = deserializer.pop_front<int>();
      }
  };

}

