#include "pch.h"
#include "Log.h"


#include <spdlog/sinks/stdout_color_sinks.h>

namespace Crave
{
	namespace Log
	{
		void Init()
		{
			//m_Logger = spdlog::stdout_color_mt("Crave");
			spdlog::set_pattern("%^[%s|%#|%!]: %v%$");
		}
	}
	//std::shared_ptr<spdlog::logger> Log::m_Logger;

	

	//std::shared_ptr<spdlog::logger> Log::GetLogger() { return m_Logger; }
}