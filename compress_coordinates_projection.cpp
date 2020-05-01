#include "compress_coordinates_projection.h"

Catalog::CompressCoordinatesProjection::CompressCoordinatesProjection(Models::MapBuilderParameters parameters) : _parameters(parameters)
{
}

void Catalog::CompressCoordinatesProjection::Project(const BusShedule& bus_schedule, std::unordered_map<std::string, Models::Stop>& stops)
{
	std::vector<Models::Stop> ordered_lon_all;
	std::vector<Models::Stop> ordered_lat_all;
	std::unordered_map<std::string, size_t> stop_to_lon_number;
	std::unordered_map<std::string, size_t> stop_to_lat_number;

	for (const auto& s : stops) {
		ordered_lon_all.push_back(s.second);
		ordered_lat_all.push_back(s.second);
	}
	std::sort(ordered_lon_all.begin(), ordered_lon_all.end(), [](const Models::Stop& lhs, const Models::Stop& rhs) {return lhs.position.lon < rhs.position.lon; });
	std::sort(ordered_lat_all.begin(), ordered_lat_all.end(), [](const Models::Stop& lhs, const Models::Stop& rhs) {return lhs.position.lat < rhs.position.lat; });

	std::unordered_set<std::string> resolved;
	size_t number = 0;
	size_t count = 0;
	for (size_t index = 0; index < ordered_lon_all.size(); ++index) {
		const auto& stop = ordered_lon_all.at(index);
		number = 0;
		for (const auto& s : resolved) {
			const auto& current = stops.at(s);
			if (AreNeighborsOnSameRoute(bus_schedule, stop, current)) {
				number = std::max(number, stop_to_lon_number.at(s) + 1);
			}
		}
		if (number > count) {
			count = number;
		}
		stop_to_lon_number[stop.name] = number;
		resolved.insert(stop.name);
	}

	double lon_step = 0.0;
	if (count > 0) {
		lon_step = (_parameters.width - 2 * _parameters.padding) / (count);
	}

	resolved.clear();
	number = 0;
	count = 0;
	for (size_t index = 0; index < ordered_lat_all.size(); ++index) {
		const auto& stop = ordered_lat_all.at(index);
		number = 0;
		for (const auto& s : resolved) {
			const auto& current = stops.at(s);
			if (AreNeighborsOnSameRoute(bus_schedule, stop, current)) {
				number = std::max(number, stop_to_lat_number.at(s) + 1);
			}
		}
		if (number > count) {
			count = number;
		}
		stop_to_lat_number[stop.name] = number;
		resolved.insert(stop.name);
	}

	double lat_step = 0.0;
	if (count > 0) {
		lat_step = (_parameters.height - 2 * _parameters.padding) / (count);
	}

	for (auto& stop : stops) {
		size_t idx = stop_to_lon_number.at(stop.first);
		stop.second.position.lon = idx * lon_step + _parameters.padding;
		idx = stop_to_lat_number.at(stop.first);
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