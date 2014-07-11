#pragma once

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

class Object
{
  public:
    typedef std::unique_ptr<Object> Pointer;
    void add_behavior( ObjectBehavior::Pointer&& behavior );

    template < typename Event >
    void dispatch( const Event& event )
    {
      m_dispatcher.dispatch( event );
    }

    template < typename Event >
    void polymorphic_dispatch( const Event& event )
    {
      m_dispatcher.polymorphic_dispatch( event );
    }

  private:
    the::ctci::Dispatcher m_dispatcher;
    the::ctci::ComponentRegistry m_components;
    std::vector< ObjectBehavior::Pointer > m_behaviors;
};

}

