#pragma once
#include "Log.h"


#define ENABLE_ASSERTS
#ifdef ENABLE_ASSERTS

#define ASSERT(x,msg) do{ if(!(x)) { LOG_ERROR("Assertion Failed");\
	std::cerr << '\t' << msg << ".\n";\
__debugbreak(); } }while(0)
#else

#define ASSERT(msg)
#endif


