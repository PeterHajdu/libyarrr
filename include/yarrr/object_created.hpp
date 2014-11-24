#pragma once

namespace yarrr
{

class ObjectCreated
{
  public:
    add_ctci( "yarrr_object_created" );

    ObjectCreated( Object::Pointer&& object )
      : object( std::move( object ) )
    {
    }

    mutable Object::Pointer object;
};

}

