#include <ostream>
#include <yarrr/hello.hpp>

namespace yarrr
{
  hello::hello( const std::string& name )
    : m_name( name )
  {
  }

  std::string
  hello::greet() const
  {
    return "hello " + m_name;
  }
}

