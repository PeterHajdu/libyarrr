#pragma once

#include <yarrr/types.hpp>
#include <string>

namespace yarrr
{
class Object;
class ObjectBehavior;

class ObjectDecorator
{
  public:
    ObjectDecorator( Object& );

    //unfortunately it can't be const because of lua export
    std::string id();
    void add_behavior_clone( ObjectBehavior& behavior );
    void move_to( const yarrr::Coordinate& );
    void set_velocity( const yarrr::Velocity& );

  private:
    Object& m_object;
};

}

