#pragma once

#include <string_view>
#include <optional>
#include <utility>
#include <vector>
#include <sstream>

class Utils
{
public:
	static std::pair<std::string_view, std::optional<std::string_view>> SplitTwoStrict(std::string_view s, std::string_view delimiter = " ");
	static std::pair<std::string_view, std::string_view> SplitTwo(std::string_view s, std::string_view delimiter = " ");
	static std::vector<std::string_view> SplitBy(std::string_view s, std::string_view delimiter);
	static double ToRadians(double degrees);
	static double ConvertToDouble(std::string_view str);
};

