#pragma once

#include <string>

namespace Nora
{
    void AssertImpl(bool value, const std::string& expr_str, const std::string& file, const std::string& func, unsigned line,
        const std::string& hint);
}

#define NORA_ASSERT(expr) Nora::AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, "")