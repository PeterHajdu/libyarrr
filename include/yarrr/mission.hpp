#pragma once

#include <yarrr/entity.hpp>
#include <yarrr/lua_engine.hpp>
#include <string>
#include <memory>
#include <functional>

namespace yarrr
{
class Serializer;
class Deserializer;

enum TaskState : char
{
  ongoing,
  failed,
  succeeded
};

class Mission final : public Entity
{
  public:
    add_polymorphic_ctci( "yarrr_mission" );
    typedef std::unique_ptr< Mission > Pointer;

    Mission() = default;
    Mission( const Mission& );
    Mission( const std::string& name, const std::string& description );

    const std::string& name() const;
    const std::string& description() const;

    class Objective;
    void add_objective( const Objective& );

    typedef std::vector< Objective > Objectives;
    const Objectives& objectives() const;

    const TaskState state() const;
    void update();

  private:
    void calculate_mission_state();

    virtual void do_serialize( Serializer& serializer ) const override final;
    virtual void do_deserialize( Deserializer& deserializer ) override final;

    std::string m_name;
    std::string m_description;
    Objectives m_objectives;
    TaskState m_state;
    sol::table m_context;
};

class Mission::Objective final : public Entity
{
  public:
    add_polymorphic_ctci( "yarrr_mission_objective" );
    typedef std::unique_ptr< Objective > Pointer;
    typedef std::function< TaskState( sol::table& ) > Updater;

    Objective() = default;
    Objective( const std::string& description, Updater updater );
    Objective( const std::string& description, sol::function updater );
    Objective( const Objective& );

    const std::string& description() const;
    const TaskState state() const;

    void update( sol::table& context );

  private:
    virtual void do_serialize( Serializer& serializer ) const override final;
    virtual void do_deserialize( Deserializer& deserializer ) override final;

    std::string m_description;
    Updater m_updater;
    TaskState m_state;
};

}

