#include "NoraPrecompiled.h"
#include "Profiler.h"

namespace Nora
{
	Profiler::Profiler(const std::string& operation, std::ostream& out)
		: start_(std::chrono::steady_clock::now()), operation_(operation), stream_(out)
	{ }


	Profiler::~Profiler()
	{
		const auto delta = std::chrono::steady_clock::now() - start_;
		stream_ << operation_ << ": "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() << " ms" << std::endl;
	}
}
