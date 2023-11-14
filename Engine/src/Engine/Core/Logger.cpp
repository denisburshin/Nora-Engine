#include "NoraPrecompiled.h"
#include "Logger.h"

namespace Nora
{	
	Logger& Logger::GetInstance()
	{
		static Logger logger;
		return logger;
	}
	
	void Logger::Log(Level level, const std::string& message) const
	{
		switch (level)
		{
		case Nora::Logger::Level::Trace:
			Trace(message);
			break;
		case Nora::Logger::Level::Info:
			Info(message);
			break;
		case Nora::Logger::Level::Warning:
			Warning(message);
			break;
		case Nora::Logger::Level::Error:
			Error(message);
			break;
		case Nora::Logger::Level::Critical:
			Critical(message);
			break;
		default:
			break;
		}
	}

	void Logger::Trace(const std::string& message) const
	{
		std::cerr << "[TRACE]: " << message << std::endl;
	}

	void Logger::Info(const std::string& message) const
	{
		std::cerr << "\033[0;32m[INFO]: " << message << std::endl;
	}

	void Logger::Warning(const std::string& message) const
	{
		std::cerr << "\033[0;33m[WARNING]: " << message << std::endl;
	}

	void Logger::Error(const std::string& message) const
	{
		std::cerr << "\033[0;31m[ERROR]: " << message << std::endl;
	}

	void Logger::Critical(const std::string& message) const
	{
		std::cerr << "\033[0;31m[CRITICAL]: " << message << std::endl;
	}
}
