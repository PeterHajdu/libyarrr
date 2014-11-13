#pragma once

#include <yarrr/types.hpp>
#include <themodel/node_list.hpp>
#include <themodel/variable.hpp>
#include <vector>

namespace yarrr
{
class Object;

class CoordinateModel
{
  public:
    CoordinateModel( const std::string& name, const Coordinate& coordinate, the::model::OwningNode& parent )
      : m_coordinate_model( name, parent )
      , m_x( "x", coordinate.x, m_coordinate_model )
      , m_y( "y", coordinate.y, m_coordinate_model )
    {
    }

  private:
    the::model::OwningNode m_coordinate_model;
    the::model::Variable< int > m_x;
    the::model::Variable< int > m_y;
};


class ObjectModel : public the::model::OwningNode
{
  public:
    ObjectModel( const Object&, the::model::OwningNode& parent );

  private:
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
    the::model::OwningNodeList m_objects_model;
};

}

