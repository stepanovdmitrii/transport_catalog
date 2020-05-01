#include "uniform_distribution_operation.h"

Catalog::UniformDistributionOperation::UniformDistributionOperation()
{
}

void Catalog::UniformDistributionOperation::Project(const BusShedule& bus_schedule, std::unordered_map<std::string, Models::Stop>& stops)
{
	std::unordered_set<std::string> fixed_stops;

	//no busses stops && transit stops
	for (const auto& stop : bus_schedule.GetStops()) {
		if (stop.second.buses.size() != 1) {
			fixed_stops.insert(stop.first);
		}
	}
	
	for (const auto& bus : bus_schedule.GetBuses()) {
		std::unordered_map<std::string, size_t> stop_to_count;

		for (size_t stop_index = 0; stop_index < bus.second.stops.size(); ++stop_index) {
			const std::string& stop = bus.second.stops.at(stop_index);
			
			if (stop_to_count.find(stop) == stop_to_count.end()) {
				stop_to_count[stop] = 0;
			}

			if (stop_index == 0) {
				stop_to_count[stop] += 3;
			}
			else if (stop_index == (bus.second.stops.size() / 2) && false == bus.second.is_roundtrip){
				
				stop_to_count[stop] += 3;
			}
			else {
				stop_to_count[stop] += 1;
			}
		}

		for (const auto& p : stop_to_count) {
			if (p.second > 2) {
				fixed_stops.insert(p.first);
			}
		}
	}

	for (const auto& bus_line : bus_schedule.GetBuses()) {
		
		for (size_t start_stop_index = 0; start_stop_index < bus_line.second.stops.size() - 1; ) {
			for (size_t current_stop_index = start_stop_index + 1; current_stop_index < bus_line.second.stops.size(); ++current_stop_index) {
				if (fixed_stops.find(bus_line.second.stops[current_stop_index]) != fixed_stops.end()) {
					const auto& start_stop_name = bus_line.second.stops[start_stop_index];
					const auto& end_stop_name = bus_line.second.stops[current_stop_index];
					const auto start_stop = stops.at(start_stop_name);
					const auto& end_stop = stops.at(end_stop_name);
					double lon_step = (stops[end_stop_name].position.lon - stops[start_stop_name].position.lon) / (current_stop_index - start_stop_index);
					double lat_step = (stops[end_stop_name].position.lat - stops[start_stop_name].position.lat) / (current_stop_index - start_stop_index);
					for (size_t index = start_stop_index + 1; index < current_stop_index; ++index) {
						const auto& stop_name = bus_line.second.stops.at(index);
						stops[stop_name].position.lon = start_stop.position.lon + lon_step * (index - start_stop_index);
						stops[stop_name].position.lat = start_stop.position.lat + lat_step * (index - start_stop_index);
					}
					start_stop_index = current_stop_index;
					break;
				}
			}
		}

	}
}
