#pragma once
#include "Log.h"


#define ENABLE_ASSERTS
#ifdef ENABLE_ASSERTS

#define ENGINE_ASSERT(x, ...) { if(!(x)) { ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else

#define ENGINE_ASSERT(x, ...)
#endif


