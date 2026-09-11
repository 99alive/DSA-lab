#pragma once

#include <string>

inline std::string csv_escape(const std::string& value)
{
    const bool needs_quotes = value.find_first_of(",\"\n") != std::string::npos;
    if (!needs_quotes) {
        return value;
    }

    std::string escaped;
    escaped.reserve(value.size() + 2);
    for (char character : value) {
        escaped += character == '"' ? "\"\"" : std::string(1, character);
    }
    return '"' + escaped + '"';
}
