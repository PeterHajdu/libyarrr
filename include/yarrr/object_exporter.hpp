#pragma once

#include <yarrr/lua_engine.hpp>
#include <yarrr/types.hpp>
#include <vector>

namespace yarrr
{
class Object;

//todo: there's plenty of space to improve the design here: model system
class ObjectModel
{
  public:
    ObjectModel( const Object&, sol::state& );
    sol::table& table();

  private:
    sol::table m_object_table;

    yarrr::Coordinate m_coordinate;
    sol::table m_coordinate_table;

    yarrr::Coordinate m_velocity;
    sol::table m_velocity_table;

    double m_orientation;
    double m_angular_velocity;
};

class ObjectContainer;

class ObjectExporter
{
  public:
    ObjectExporter( const ObjectContainer&, sol::state& );
    void refresh();

  private:
    void reset_objects_table();

    sol::state& m_lua;
    sol::table m_objects;
    const ObjectContainer& m_container;
    std::vector< ObjectModel > m_object_models;
};

}

