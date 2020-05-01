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
