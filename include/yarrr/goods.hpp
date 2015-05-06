#pragma once
#include <string>

namespace yarrr
{

class Goods
{
  public:
    using amount_type = uint32_t;
    Goods(
        std::string name,
        std::string type,
        amount_type amount )
      : name( std::move( name ) )
      , type( std::move( type ) )
      , amount( amount )
    {
    }

    Goods( const Goods& other )
      : name( other.name )
      , type( other.type )
      , amount( other.amount )
    {
    }

    Goods( Goods&& other )
      : name( std::move( other.name ) )
      , type( std::move( other.type ) )
      , amount( other.amount )
    {
    }

    const std::string name;
    const std::string type;
    const amount_type amount;
};

inline
bool
operator==( const Goods& l, const Goods& r )
{
  return
    l.name == r.name &&
    l.type == r.type;
}

}

