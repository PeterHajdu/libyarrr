#pragma once

#include <thelog/logger.hpp>
#include <thelog/trace.hpp>

namespace yarrr
{

namespace log
{

enum : int
{
  error = 200,
  warning = 150,
  info = 100,
  debug = 50,
  trace = 0
};

}

}

