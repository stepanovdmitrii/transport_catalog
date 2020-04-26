#include "utils.h"

std::pair<std::string_view, std::optional<std::string_view>> Utils::SplitTwoStrict(std::string_view s, std::string_view delimiter)
{
    const size_t pos = s.find(delimiter);
    if (pos == s.npos) {
        return { s, std::nullopt };
    }
    else {
        return { s.substr(0, pos), s.substr(pos + delimiter.length()) };
    }
}

std::pair<std::string_view, std::string_view> Utils::SplitTwo(std::string_view s, std::string_view delimiter)
{
    const auto [lhs, rhs_opt] = SplitTwoStrict(s, delimiter);
    return { lhs, rhs_opt.value_or("") };
}

std::vector<std::string_view> Utils::SplitBy(std::string_view s, std::string_view delimiter)
{
    std::vector<std::string_view> result;
    std::string_view current = s;
    size_t pos = current.find(delimiter);
    while (pos != current.npos)
    {
        result.push_back(current.substr(0, pos));
        current = current.substr(pos + delimiter.length());
        pos = current.find(delimiter);
    }
    result.push_back(current);
    return result;
}

double Utils::ToRadians(double degrees)
{
    return degrees * 3.1415926535 / 180;
}

double Utils::ConvertToDouble(std::string_view str)
{
    std::stringstream ss;
    ss << str;
    double result = 0.0;
    ss >> result;
    return result;
}
