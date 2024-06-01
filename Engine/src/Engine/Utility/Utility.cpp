#include "Utility.h"
#include "NoraPrecompiled.h"

#include <fstream>
#include <sstream>

namespace Nora::Utility
{
	std::string ReadFile(const std::string& file)
	{
		std::ifstream in(file, std::ios::ate | std::ios::binary);

		if (!in)
		{
			NORA_WARNING("File {} cannot be open!", file);
			return {};
		}

		std::stringstream stream;
		stream << in.rdbuf();
		return stream.str();
	}
}
