#pragma once

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
		void Log(Level level, const std::string& message) const;

		Logger(const Logger& other) = delete;
		Logger& operator=(const Logger& other) = delete;

	private:
		Logger() = default;

		void Trace(const std::string& message) const;
		void Info(const std::string& message) const;
		void Warning(const std::string& message) const;
		void Error(const std::string& message) const;
		void Critical(const std::string& message) const;
	};
}


#ifdef NORA_DEBUG
	#define NORA_LOG(level, message) Nora::Logger::GetInstance().Log(level, message)
#else
	#define NORA_LOG(level, message)
#endif

#define NORA_TRACE(message) NORA_LOG(Nora::Logger::Level::Trace, message)
#define NORA_INFO(message) NORA_LOG(Nora::Logger::Level::Info, message)
#define NORA_WARNING(message) NORA_LOG(Nora::Logger::Level::Warning, message)
#define NORA_ERROR(message) NORA_LOG(Nora::Logger::Level::Error, message)
#define NORA_CRITICAL(message) NORA_LOG(Nora::Logger::Level::Critical, message)


