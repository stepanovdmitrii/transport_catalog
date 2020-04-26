#include "find_way_operation.h"

Operations::FindWay::FindWay(std::string from, std::string to, size_t request_id) : _from(from), _to(to), _request_id(request_id)
{
}

void Operations::FindWay::Do(Catalog::BusShedule& bus_schedule, IO::IFormatter& formatter)
{
    auto info = bus_schedule.FindOptimalWay(_from, _to);
    info.request_id = _request_id;
    formatter.Write(info);
}
