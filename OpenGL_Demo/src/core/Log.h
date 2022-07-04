#pragma once

#include <memory>

#include "spdlog/spdlog.h"

class Log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetLogger()   { return s_Logger; }
public:

private:
	static std::shared_ptr<spdlog::logger> s_Logger;

};


#define LOG_TRACE(...)	      Log::GetLogger->trace(__VA_ARGS__)
#define LOG_INFO(...)	      Log::GetLogger->info (__VA_ARGS__)
#define LOG_WARN(...)	      Log::GetLogger->warn (__VA_ARGS__)
#define LOG_ERROR(...)	      Log::GetLogger->error(__VA_ARGS__)
#define LOG_FATAL(...)	      Log::GetLogger->fatal(__VA_ARGS__) 

#ifdef _DEBUG
	#define LOG_DEBUG_TRACE(...)	  Log::GetLogger->trace(__VA_ARGS__)
	#define LOG_DEBUG_INFO(...)		  Log::GetLogger->info (__VA_ARGS__)
	#define LOG_DEBUG_WARN(...)	      Log::GetLogger->warn (__VA_ARGS__)
	#define LOG_DEBUG_ERROR(...)	  Log::GetLogger->error(__VA_ARGS__)
	#define LOG_DEBUG_FATAL(...)	  Log::GetLogger->fatal(__VA_ARGS__)
#else
	#define DEBUG_TRACE(...)
	#define DEBUG_INFO(...)
	#define DEBUG_WARN(...)
	#define DEBUG_ERROR(...)
	#define DEBUG_FATAL(...)
#endif
