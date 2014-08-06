#pragma once

#include <yarrr/entity.hpp>
#include <thectci/factory.hpp>
#include <memory>
#include <vector>

namespace yarrr
{

class EntityFactory
{
  public:
    static EntityFactory& get();

    template < typename T >
    static void register_class()
    {
      EntityFactory& instance( get() );
      instance.m_creators.emplace_back( new the::ctci::ExactCreator<Entity,T>() );
      instance.m_factory.register_creator( T::ctci, *instance.m_creators.back() );
    }

    static Entity::Pointer create( Deserializer& );
    static Entity::Pointer create( const Data& );

    static bool is_registered( the::ctci::Id id );

  private:
    the::ctci::Factory< yarrr::Entity > m_factory;

    typedef the::ctci::Creator< Entity > Creator;
    std::vector< std::unique_ptr< Creator> > m_creators;
};

template < typename Entity >
class AutoEntityRegister
{
  public:
    AutoEntityRegister()
    {
      EntityFactory::register_class<Entity>();
    }
};

}

