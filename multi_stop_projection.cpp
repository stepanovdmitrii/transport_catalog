#include "multi_stop_projection.h"

Catalog::MultiStopProjection::MultiStopProjection(std::vector<std::shared_ptr<IStopProjection>> projections): _projections(projections)
{
}

void Catalog::MultiStopProjection::Project(const BusShedule& bus_schedule, std::unordered_map<std::string, Models::Stop>& stops)
{
	for (auto proj : _projections) {
		proj->Project(bus_schedule, stops);
	}
}
