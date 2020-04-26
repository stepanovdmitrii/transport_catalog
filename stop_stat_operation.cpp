#include "stop_stat_operation.h"

Operations::StopStat::StopStat(std::string stop, size_t request_id) : _stop(stop), _request_id(request_id)
{
}

void Operations::StopStat::Do(Catalog::BusShedule& bus_schedule, IO::IFormatter& formatter)
{
    Models::StopStatInfo info = Collect(bus_schedule);
    info.request_id = _request_id;
    formatter.Write(info);
}

Models::StopStatInfo Operations::StopStat::Collect(Catalog::BusShedule& bus_schedule)
{
    Models::StopStatInfo info;
    info.stop = _stop;
    auto range = bus_schedule.FindStopBuses(_stop);
    if (range) {
        info.found = true;
        if (range->begin() != range->end()) {
            for (auto it = range->begin(); it != range->end(); ++it) {
                info.buses.push_back(*it);
            }
        }
    }
    return std::move(info);
}
