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
    typedef uint64_t Id;

    class Info
    {
      public:
        Info() = default;
        Info( const std::string& name, const std::string& description )
          : name( name )
          , description( description )
        {
        }

        std::string name;
        std::string description;
    };

    Mission();
    Mission( const Mission& );
    Mission( const Info& );

    Id id() const;

    static Pointer create( const Info& info );

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

    Info m_info;
    Objectives m_objectives;
    TaskState m_state;
    Id m_id;
};

class Mission::Objective final : public Entity
{
  public:
    add_polymorphic_ctci( "yarrr_mission_objective" );
    typedef std::unique_ptr< Objective > Pointer;
    typedef std::function< TaskState( const std::string& mission_id ) > Updater;

    Objective() = default;
    Objective( const std::string& description, Updater updater );
    Objective( const std::string& description, sol::function updater );
    Objective( const Objective& );

    const std::string& description() const;
    const TaskState state() const;

    void update( const std::string& mission_id );

  private:
    virtual void do_serialize( Serializer& serializer ) const override final;
    virtual void do_deserialize( Deserializer& deserializer ) override final;

    std::string m_description;
    Updater m_updater;
    TaskState m_state;
};

}

