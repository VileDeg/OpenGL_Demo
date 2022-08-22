#pragma once
#include <iostream>
#include <cstring>

namespace Crave
{

#define __FILENAME__ (strrchr("\\" __FILE__, '\\') + 1)

#define LOG_ERROR(msg) do{\
	std::cerr << "ERROR: FILE: " << __FILENAME__ << ", LINE: "\
	<< __LINE__ << ", FUNC: " << __func__ << ": " << msg << ".\n"; \
} while(0)
}