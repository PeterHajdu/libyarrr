#pragma once

#include <memory>
#include <themodel/node.hpp>
#include <themodel/variable.hpp>

namespace yarrr
{

class Character
{
  public:
    typedef std::unique_ptr< Character > Pointer;
    Character( const std::string& object_id, the::model::OwningNode& parent );

  private:
    the::model::OwningNode m_character_model;
    the::model::Variable< std::string > m_object_id;
};

}

