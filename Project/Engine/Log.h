#pragma once
#define SPDLOG_WCHAR_TO_UTF8_SUPPORT
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Modot {
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return s_EngineLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_EngineLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}
// Engine Log
#define MD_ENGINE_ERROR(...) ::Modot::Log::GetEngineLogger()->error(__VA_ARGS__)
#define MD_ENGINE_WARN(...) ::Modot::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define MD_ENGINE_INFO(...) ::Modot::Log::GetEngineLogger()->info(__VA_ARGS__)
#define MD_ENGINE_TRACE(...) ::Modot::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define MD_ENGINE_FATAL(...) ::Modot::Log::GetEngineLogger()->fatal(__VA_ARGS__)

// Client Log
#define MD_ERROR(...) ::Modot::Log::GetClientLogger()->error(__VA_ARGS__)
#define MD_WARN(...) ::Modot::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MD_INFO(...) ::Modot::Log::GetClientLogger()->info(__VA_ARGS__)
#define MD_TRACE(...) ::Modot::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MD_FATAL(...) ::Modot::Log::GetClientLogger()->fatal(__VA_ARGS__)