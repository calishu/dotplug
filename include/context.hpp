#pragma once

#include <string>

struct Context {
  std::string name;
  bool forced;
};

extern Context* ctx;
