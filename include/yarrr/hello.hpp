#pragma once

#include <ostream>

namespace yarrr
{
  class hello
  {
    public:
      hello( const std::string& name );
      std::string greet() const;

    private:
      const std::string m_name;
  };
}

