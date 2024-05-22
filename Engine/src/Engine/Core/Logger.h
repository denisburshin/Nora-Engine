#pragma once
#include <format>

namespace Nora
{
	class Logger
	{
	public:
		enum class Level
		{
			Trace, Info, Warning, Error, Critical
		};

		static Logger& GetInstance();

		template <class... Args>
		void Log(Level level, std::string_view format, Args&&... args) const;

		Level GetCurrentLevel() const;

		Logger(const Logger& other) = delete;
		Logger& operator=(const Logger& other) = delete;

	private:
		Logger();

		template <class... Args>
		void Info(std::string_view format, Args&&... args) const;

		template <class... Args>
		void Trace(std::string_view format, Args&&... args) const;

		template <class... Args>
		void Warning(std::string_view format, Args&&... args) const;

		template <class... Args>
		void Error(std::string_view format, Args&&... args) const;

		template <class... Args>
		void Critical(std::string_view format, Args&&... args) const;
	private:
		Level level_;
	};

	template <class... Args>
	void Logger::Log(Level level, std::string_view format, Args&&... args) const
	{
		if (level < level_)
			return;

		switch (level)
		{
		case Nora::Logger::Level::Trace:
			Trace(format, args...);
			break;
		case Nora::Logger::Level::Info:
			Info(format, args...);
			break;
		case Nora::Logger::Level::Warning:
			Warning(format, args...);
			break;
		case Nora::Logger::Level::Error:
			Error(format, args...);
			break;
		case Nora::Logger::Level::Critical:
			Critical(format, args...);
			break;
		default:
			break;
		}

		using namespace std::string_view_literals;
		std::cerr << "\033[0m"sv;
	}

	//TODO: A lot of duplication below. Fix

	template <class... Args>
	void Logger::Info(std::string_view format, Args&&... args) const
	{
		using namespace std::string_view_literals;
		std::cerr << "[INFO]: "sv << std::vformat(format, std::make_format_args(std::forward<Args>(args)...)) << std::endl;
	}

	template <class... Args>
	void Logger::Trace(std::string_view format, Args&&... args) const
	{
		using namespace std::string_view_literals;
		std::cerr << "[TRACE]: "sv << std::vformat(format, std::make_format_args(std::forward<Args>(args)...)) << std::endl;
	}

	template <class... Args>
	void Logger::Warning(std::string_view format, Args&&... args) const
	{
		using namespace std::string_view_literals;
		std::cerr << "[WARNING]: "sv << std::vformat(format, std::make_format_args(std::forward<Args>(args)...)) << std::endl;
	}

	template <class... Args>
	void Logger::Error(std::string_view format, Args&&... args) const
	{
		using namespace std::string_view_literals;
		std::cerr << "[ERROR]: "sv << std::vformat(format, std::make_format_args(std::forward<Args>(args)...)) << std::endl;
	}

	template <class... Args>
	void Logger::Critical(std::string_view format, Args&&... args) const
	{
		using namespace std::string_view_literals;
		std::cerr << "[CRITICAL]: "sv << std::vformat(format, std::make_format_args(std::forward<Args>(args)...)) << std::endl;
	}
}

#ifdef NORA_DEBUG
	#define NORA_LOG(level, format, ...) Nora::Logger::GetInstance().Log(level, format, __VA_ARGS__)
#else
	#define NORA_LOG(level, message)
#endif

#define NORA_TRACE(format, ...) NORA_LOG(Nora::Logger::Level::Trace, format, __VA_ARGS__)
#define NORA_INFO(format, ...) NORA_LOG(Nora::Logger::Level::Info, format, __VA_ARGS__)
#define NORA_WARNING(format, ...) NORA_LOG(Nora::Logger::Level::Warning, format, __VA_ARGS__)
#define NORA_ERROR(format, ...) NORA_LOG(Nora::Logger::Level::Error, format, __VA_ARGS__)
#define NORA_CRITICAL(format, ...) NORA_LOG(Nora::Logger::Level::Critical, format, __VA_ARGS__)


