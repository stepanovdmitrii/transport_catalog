#include "compress_coordinates_projection.h"

Catalog::CompressCoordinatesProjection::CompressCoordinatesProjection(Models::MapBuilderParameters parameters) : _parameters(parameters)
{
}

void Catalog::CompressCoordinatesProjection::Project(const BusShedule& bus_schedule, std::unordered_map<std::string, Models::Stop>& stops)
{
	std::vector<Models::Stop> ordered_lon_all;
	std::vector<Models::Stop> ordered_lat_all;

	for (const auto& s : stops) {
		ordered_lon_all.push_back(s.second);
		ordered_lat_all.push_back(s.second);
	}
	std::sort(ordered_lon_all.begin(), ordered_lon_all.end(), [](const Models::Stop& lhs, const Models::Stop& rhs) {return lhs.position.lon < rhs.position.lon; });
	std::sort(ordered_lat_all.begin(), ordered_lat_all.end(), [](const Models::Stop& lhs, const Models::Stop& rhs) {return lhs.position.lat < rhs.position.lat; });

	std::vector<Models::Stop> ordered_lon = RemoveNeighbors(bus_schedule, ordered_lon_all);
	std::vector<Models::Stop> ordered_lat = RemoveNeighbors(bus_schedule, ordered_lat_all);

	double lon_step = 0.0;
	if (ordered_lon.size() > 1) {
		lon_step = (_parameters.width - 2 * _parameters.padding) / (ordered_lon.size() - 1);
	}
	double lat_step = 0.0;
	if (ordered_lat.size() > 1) {
		lat_step = (_parameters.height - 2 * _parameters.padding) / (ordered_lat.size() - 1);
	}

	std::vector<double> ordered_lon_values;
	std::vector<double> ordered_lat_values;

	for (const auto& p : ordered_lon) {
		ordered_lon_values.push_back(p.position.lon);
	}

	for (const auto& p : ordered_lat) {
		ordered_lat_values.push_back(p.position.lat);
	}

	for (auto& stop : stops) {
		auto lon_ub = std::upper_bound(ordered_lon_values.begin(), ordered_lon_values.end(), stop.second.position.lon);
		auto lon_it = std::prev(lon_ub);
		size_t idx = std::distance(ordered_lon_values.begin(), lon_it);
		stop.second.position.lon = idx * lon_step + _parameters.padding;
		auto lat_ub = std::upper_bound(ordered_lat_values.begin(), ordered_lat_values.end(), stop.second.position.lat);
		auto lat_it = std::prev(lat_ub);
		idx = std::distance(ordered_lat_values.begin(), lat_it);
		stop.second.position.lat = _parameters.height - _parameters.padding - idx * lat_step;
	}

}

bool Catalog::CompressCoordinatesProjection::AreNeighborsOnSameRoute(const Catalog::BusShedule& bus_schedule, const Models::Stop& first, const Models::Stop& second) const
{
	std::set<std::string> common_busses;

	for (const auto& from_first : first.buses) {
		for (const auto& from_secod : second.buses) {
			if (from_first == from_secod) {
				common_busses.insert(from_first);
			}
		}
	}
	if (common_busses.size() == 0) {
		return false;
	}

	std::string prev;
	for (const std::string& bus : common_busses) {
		auto bus_stops = bus_schedule.FindBusStops(bus);
		if (false == bus_stops.has_value()) {
			continue;
		}
		prev = "";
		for (const auto& s : bus_stops.value()) {
			if ((s == first.name) && (prev == second.name)) {
				return true;
			}
			if ((s == second.name) && (prev == first.name)) {
				return true;
			}
			prev = s;
		}
	}
	return false;
}

std::vector<Models::Stop> Catalog::CompressCoordinatesProjection::RemoveNeighbors(const Catalog::BusShedule& bus_schedule, std::vector<Models::Stop>& ordered) const
{
	if (ordered.size() > 1) {
		std::vector<Models::Stop> result;

		for (size_t current = 0; current < ordered.size();) {
			result.push_back(ordered[current]);
			std::vector<Models::Stop> current_group;
			current_group.push_back(ordered[current]);

			for (size_t next = current + 1; next < ordered.size(); ++next) {
				bool hasDirectRoute = false;

				for (const auto& stop : current_group) {
					hasDirectRoute = AreNeighborsOnSameRoute(bus_schedule, stop, ordered[next]);
					if (hasDirectRoute) {
						break;
					}
				}

				if (hasDirectRoute) {
					break;
				}
				else {
					current_group.push_back(ordered[next]);
				}
			}

			current = current + current_group.size();
		}
		return std::move(result);
	}
	else {
		return std::move(std::vector<Models::Stop>(ordered));
	}
}
