#include "add_stop_operation.h"

Operations::AddStop::AddStop(Models::StopInfo info): _info(info)
{
}

void Operations::AddStop::Do(Catalog::BusShedule& bus_schedule)
{
	bus_schedule.AddStop(_info);
}
