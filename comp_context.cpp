#include "comp_context.h"

/*
Svg::Point Catalog::ComputationContext::GetProjection(const Models::Position& position) const
{
	Svg::Point p;
	auto lon_ub = std::upper_bound(_ordered_lon.begin(), _ordered_lon.end(), position.lon);
	auto lon_it = std::prev(lon_ub);
	size_t idx = std::distance(_ordered_lon.begin(), lon_it);
	p.x = idx * _x_step + _padding;
	auto lat_ub = std::upper_bound(_ordered_lat.begin(), _ordered_lat.end(), position.lat);
	auto lat_it = std::prev(lat_ub);
	idx = std::distance(_ordered_lat.begin(), lat_it);
	p.y = _height - _padding - idx * _y_step;
	return std::move(p);
}
*/

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
