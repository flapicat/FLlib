#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace FLlib
{
	class Log
	{
	public:
		static void init();
		inline static std::shared_ptr<spdlog::logger>& GetLogger()
		{
			if (!s_Logger)
				init();
			return s_Logger;
		}
	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};

}

#define LOG_TRACE(...)     ::FLlib::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)      ::FLlib::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)      ::FLlib::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)     ::FLlib::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...)     ::FLlib::Log::GetLogger()->critical(__VA_ARGS__)
