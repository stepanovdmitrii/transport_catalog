#include "add_bus_operation.h"

Operations::AddBus::AddBus(Models::BusInfo info): _info(info)
{
}

void Operations::AddBus::Do(Catalog::BusShedule& bus_schedule)
{
	bus_schedule.AddBus(_info);
}
