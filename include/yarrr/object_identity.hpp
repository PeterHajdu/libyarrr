#pragma once

#include <yarrr/object_behavior.hpp>

namespace yarrr
{
class Serializer;
class Deserializer;

class ObjectIdentity : public ObjectBehavior
{
  public:
    add_polymorphic_ctci( "yarrr_object_identity" );
    ObjectIdentity();
    ObjectIdentity( std::string captain );
    ObjectIdentity( const ObjectIdentity& );
    virtual Pointer clone() const override;

    const std::string& captain() const;

  private:
    virtual void serialize_behavior( Serializer& serializer ) const override;
    virtual void deserialize_behavior( Deserializer& deserializer ) override;

    std::string m_captain;
};

}

