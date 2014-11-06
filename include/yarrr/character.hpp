#pragma once

#include <memory>
#include <yarrr/lua_engine.hpp>

namespace yarrr
{
class Object;

class Character
{
  public:
    typedef std::unique_ptr< Character > Pointer;
    Character( sol::state& );
    void assign_object( const Object& );

    sol::table& table();

  private:
    sol::table m_table;
    std::string m_object_id;
};

}

