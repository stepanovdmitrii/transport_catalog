#include "set_route_parameters_operation.h"

Operations::SetRouteParameters::SetRouteParameters(double wait_time, double velocity): _wait_time(wait_time), _velocity(velocity)
{
}

void Operations::SetRouteParameters::Do(Catalog::BusShedule& bus_schedule)
{
	bus_schedule.SetRouteParameters(_wait_time, _velocity);
}
