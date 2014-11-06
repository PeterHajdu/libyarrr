#pragma once

#include <memory>

namespace yarrr
{

class Character
{
  public:
    typedef std::unique_ptr< Character > Pointer;
};

}

