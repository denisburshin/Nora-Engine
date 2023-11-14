#pragma once

#include <iostream>
#include <chrono>
#include <string>

namespace Nora
{
	class Profiler
	{
	public:
		Profiler(const std::string& operation, std::ostream& out = std::cerr);
		~Profiler();
	private:
		const std::chrono::steady_clock::time_point start_;
		const std::string operation_;
		std::ostream& stream_;
	};
}

#if defined(NORA_DEBUG) || defined(NORA_PROFILE)
	#define CONCAT_INTERNAL(x, y) x ## y
	#define PROFILE_CONCAT(x, y) CONCAT_INTERNAL(x, y)
	#define UNIQUE_PROFILER_NAME PROFILE_CONCAT(profiler_guard, __LINE__)
	#define NORA_PROFILER_NAME(x) Nora::Profiler UNIQUE_PROFILER_NAME(x)
	#define NORA_PROFILER(x) NORA_PROFILER_NAME(x)
#else
	#define NORA_PROFILER(x, y)
#endif
