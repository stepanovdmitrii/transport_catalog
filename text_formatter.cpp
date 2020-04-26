#include "text_formatter.h"

IO::TextFormatter::TextFormatter(std::ostream& output) : _output(output)
{
}

void IO::TextFormatter::Write(const Models::BusStatInfo& info)
{
    if (info.found) {
        _output << "Bus " << info.bus << ": "
            << info.total_stops << " stops on route, "
            << info.unique_stops << " unique stops, "
            << info.length << " route length, "
            << info.curvature << " curvature" << std::endl;
    }
    else {
        _output << "Bus " << info.bus << ": not found" << std::endl;
    }
}

void IO::TextFormatter::Write(const Models::StopStatInfo& info)
{
    if (info.found) {
        auto it = info.buses.begin();
        if (it == info.buses.end()) {
            _output << "Stop " << info.stop << ": no buses" << std::endl;
        }
        else {
            _output << "Stop " << info.stop << ": buses";

            while (it != info.buses.end()) {
                _output << " " << *it;
                ++it;
            }
            _output << std::endl;
        }
    }
    else {
        _output << "Stop " << info.stop << ": not found" << std::endl;
    }
}

void IO::TextFormatter::Write(const Models::RouteInfo& info)
{
    throw std::runtime_error("not implemented");
}

void IO::TextFormatter::Flush()
{
    return;
}

void IO::TextFormatter::Write(const Models::DrawMapInfo& info)
{
}
