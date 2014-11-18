#pragma once

#include <thelog/logger.hpp>
#include <thelog/trace.hpp>

namespace yarrr
{

namespace log
{

enum : int
{
  error = 1200,
  warning = 1100,
  info = 1000,
  debug = 900,
  trace = 800,
  insane = 700
};

}

}

