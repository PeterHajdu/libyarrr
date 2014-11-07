#pragma once

#include <yarrr/object.hpp>
#include <thectci/id.hpp>
#include <functional>
#include <unordered_map>
#include <themodel/function.hpp>
#include <themodel/node.hpp>

namespace yarrr
{

class ObjectFactory final
{
  public:
    add_ctci( "yarrr_object_factory" )
    typedef std::function< yarrr::Object::Pointer() > Creator;

    ObjectFactory();
    yarrr::Object::Pointer create_a( const std::string& key );
    void register_creator( const std::string& key, Creator creator );

  private:
    typedef std::unordered_map< std::string, Creator > Creators;
    Creators m_creators;

    the::model::Node m_factory_model;
    the::model::Function m_register_model;
};

}

