#pragma once

#include <thelog/logger.hpp>
#include <thelog/trace.hpp>

namespace yarrr
{

namespace log
{

enum : int
{
  error = 100,
  warning = 50,
  info = 0,
  debug = -50
};

}

}

