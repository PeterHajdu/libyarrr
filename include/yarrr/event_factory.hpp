#pragma once

#include <yarrr/event.hpp>
#include <thectci/factory.hpp>
#include <memory>
#include <vector>

namespace yarrr
{

class EventFactory
{
  public:
    static EventFactory& get();

    template < typename T >
    static void register_class()
    {
      EventFactory& instance( get() );
      instance.m_creators.emplace_back( new the::ctci::ExactCreator<Event,T>() );
      instance.m_factory.register_creator( T::ctci, *instance.m_creators.back() );
    }

    static Event::Pointer create( const Data& );

    static void destroy();

  private:
    the::ctci::Factory< yarrr::Event > m_factory;

    typedef the::ctci::Creator< Event > Creator;
    std::vector< std::unique_ptr< Creator> > m_creators;
};

template < typename Event >
class AutoEventRegister
{
  public:
    AutoEventRegister()
    {
      EventFactory::register_class<Event>();
    }
};

}

