#include "ProtoBufDeserializer.h"

Operations::ProtoBufDeserializer::ProtoBufDeserializer(std::string file): _file(file)
{
}

void Operations::ProtoBufDeserializer::Deserialize(Catalog::BusShedule& bus_shedule)
{
	TransportCatalog catalog;
	std::ifstream in(_file, std::ios_base::binary);
	catalog.ParsePartialFromIstream(&in);
	in.close();
	for (int bus_index = 0; bus_index < catalog.buses_size(); ++bus_index) {
		const Bus& bus = catalog.buses(bus_index);
		Models::BusInfo info;
		info.name = bus.name();
		info.is_roundtrip = bus.is_roundtrip();
		for (int stop_index = 0; stop_index < bus.stops_size(); ++stop_index) {
			info.stops.push_back(bus.stops(stop_index));
		}
		bus_shedule.AddBus(info);
	}
	for (int stop_index = 0; stop_index < catalog.stops_size(); ++stop_index) {
		const Stop& stop = catalog.stops(stop_index);
		Models::StopInfo info;
		info.name = stop.name();
		info.position.lat = stop.position().lat();
		info.position.lon = stop.position().lon();
		for (const auto& p : stop.known_distances()) {
			info.known_distance[p.stop()] = p.distance();
		}
		bus_shedule.AddStop(info);
	}
}
