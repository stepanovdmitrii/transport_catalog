#include "bus_stat_operation.h"

Operations::BusStat::BusStat(std::string bus, size_t request_id): _bus(bus), _request_id(request_id)
{
}

void Operations::BusStat::Do(Catalog::BusShedule& bus_schedule, IO::IFormatter& formatter)
{
    Models::BusStatInfo info = Collect(bus_schedule);
    info.request_id = _request_id;
    formatter.Write(info);
}

Models::BusStatInfo Operations::BusStat::Collect(Catalog::BusShedule& bus_schedule)
{
    Models::BusStatInfo info;
    info.bus = _bus;
    auto stops = bus_schedule.FindBusStops(_bus);
    if (stops) {
        info.found = true;
        double geo_length = 0.00;
        std::unordered_set<std::string> uniq_stops;
        Models::Position prev_position;
        std::string prev_stop;
        bool first = true;

        for (auto it = stops->begin(); it != stops->end(); ++it) {
            info.total_stops++;
            Models::Position p = FindPosition(*it, bus_schedule);
            if (!first) {
                geo_length += prev_position.Distance(p);
                info.length += FindRouteDistance(prev_stop, *it, bus_schedule);
            }
            uniq_stops.insert(*it);
            prev_position = p;
            prev_stop = *it;
            first = false;
        }
        info.unique_stops = uniq_stops.size();
        info.curvature = info.length / geo_length;
    }
    return info;
}

Models::Position Operations::BusStat::FindPosition(const std::string& stop, const Catalog::BusShedule& bus_schedule)
{
    auto it = _known_stops.find(stop);
    if (it == _known_stops.end()) {
        Models::Position p = bus_schedule.GetStopPosition(stop);
        _known_stops[stop] = p;
        return p;
    }
    return it->second;
}

double Operations::BusStat::FindRouteDistance(const std::string& first_stop, const std::string& second_stop, const Catalog::BusShedule& bus_schedule)
{
    auto it = _known_distances.find(first_stop);
    if (it != _known_distances.end()) {
        auto it2 = it->second.find(second_stop);
        if (it2 != it->second.end()) {
            return it2->second;
        }
    }
    double d = bus_schedule.GetRouteDistance(first_stop, second_stop);
    _known_distances[first_stop][second_stop] = d;
    return d;
}
