#include "Log.h"

#include"spdlog/sinks/stdout_color_sinks.h"

namespace FLlib
{
	std::shared_ptr<spdlog::logger> Log::s_Logger;

	void Log::init()
	{
		if (s_Logger)
			return; // already initialized (spdlog throws if the same logger name is registered twice)

		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_Logger = spdlog::stdout_color_mt("FLlib");
		s_Logger->set_level(spdlog::level::trace);
	}
}