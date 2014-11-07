#pragma once

#include <memory>
#include <themodel/node.hpp>
#include <themodel/variable.hpp>

namespace yarrr
{
class Object;

class Character
{
  public:
    typedef std::unique_ptr< Character > Pointer;
    Character( the::model::Node& parent );
    void assign_object( const Object& );

  private:
    the::model::Node m_character_model;
    the::model::Variable< std::string > m_object_id;
};

}

