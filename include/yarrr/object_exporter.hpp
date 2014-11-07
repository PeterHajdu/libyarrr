#pragma once

#include <yarrr/types.hpp>
#include <themodel/node.hpp>
#include <themodel/variable.hpp>
#include <vector>

namespace yarrr
{
class Object;

class CoordinateModel
{
  public:
    CoordinateModel( const std::string& name, const Coordinate& coordinate, the::model::Node& parent )
      : m_coordinate_model( name, parent )
      , m_x( "x", coordinate.x, m_coordinate_model )
      , m_y( "y", coordinate.y, m_coordinate_model )
    {
    }

  private:
    the::model::Node m_coordinate_model;
    the::model::Variable< int > m_x;
    the::model::Variable< int > m_y;
};


class ObjectModel
{
  public:
    typedef std::unique_ptr< ObjectModel > Pointer;
    ObjectModel( const Object&, the::model::Node& parent );

  private:
    the::model::Node m_object_model;
    CoordinateModel m_coordinate;
    CoordinateModel m_velocity;
    the::model::Variable< double > m_orientation;
    the::model::Variable< double > m_angular_velocity;
};

class ObjectContainer;

class ObjectExporter
{
  public:
    ObjectExporter( const ObjectContainer&, the::model::Lua& );
    void refresh();

  private:
    void add_model_of( const yarrr::Object& );

    const ObjectContainer& m_container;
    the::model::Node m_objects_model;
    std::vector< ObjectModel::Pointer > m_object_models;
};

}

