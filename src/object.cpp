#include <yarrr/object.hpp>

namespace yarrr
{

void
Object::add_behavior( ObjectBehavior::Pointer&& behavior )
{
  behavior->register_to( *this, m_components );
  m_behaviors.emplace_back( std::move( behavior ) );
}

}

