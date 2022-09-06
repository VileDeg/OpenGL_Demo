#pragma once
#include <spdlog/spdlog.h>

namespace Crave
{
	namespace Log
	{
		void Init();

//		static std::shared_ptr<spdlog::logger> GetLogger();
	//	static std::shared_ptr<spdlog::logger> m_Logger;
	};

#define FILENAME (strrchr("\\" __FILE__, '\\') + 1)

#define STR_INDIR(x) #x
#define STR(x) STR_INDIR(x)

#define LOG_INFO(...) do{\
	SPDLOG_INFO(__VA_ARGS__);\
} while(0)
#define LOG_WARN(...) do{\
	SPDLOG_WARN(__VA_ARGS__);\
} while(0)
#define LOG_ERROR(...) do{\
	SPDLOG_ERROR(__VA_ARGS__);\
} while(0)
#define LOG_FATAL(...) do{\
	SPDLOG_CRITICAL(__VA_ARGS__);\
} while(0)
}

