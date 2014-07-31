#pragma once

#include <thectci/dispatcher.hpp>
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

class Object final : public the::ctci::Dispatcher
{
  public:
    typedef uint64_t Id;
    typedef std::unique_ptr<Object> Pointer;
    void add_behavior( ObjectBehavior::Pointer&& behavior );

  private:
    the::ctci::ComponentRegistry m_components;
    std::vector< ObjectBehavior::Pointer > m_behaviors;
};

}

