#include "comp_context.h"

Catalog::ComputationContext::ComputationContext(std::unordered_map<std::string, Models::Stop>&& stops) : _stops(std::move(stops))
{
}

Svg::Point Catalog::ComputationContext::GetStopPosition(const std::string& stop) const
{
	Svg::Point p;
	p.x = _stops.at(stop).position.lon;
	p.y = _stops.at(stop).position.lat;
	return p;
}

void Catalog::ComputationContext::SetBusIndex(const std::string& bus, size_t index)
{
	_bus_indexes[bus] = index;
}

size_t Catalog::ComputationContext::GetBusIndex(const std::string& bus) const
{
	return _bus_indexes.at(bus);
}
