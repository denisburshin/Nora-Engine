#include "NoraPrecompiled.h"
#include "Logger.h"

namespace Nora
{	
	Logger::Logger()
		: level_(Logger::Level::Trace)
	{ }

	Logger& Logger::GetInstance()
	{
		static Logger logger;
		return logger;
	}

	Logger::Level Logger::GetCurrentLevel() const
	{
		return level_;
	}
}
