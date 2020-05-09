#include "ProtoBufSerializer.h"

Operations::ProtoBufSerializer::ProtoBufSerializer(std::string file): _file(file)
{
}

void Operations::ProtoBufSerializer::Serialize(const Catalog::BusShedule& bus_schedule)
{
	TransportCatalog catalog;
	for (const auto& stop : bus_schedule.GetStops()) {
		Stop* s = catalog.add_stops();
		s->set_name(stop.second.name);
		s->mutable_position()->set_lat(stop.second.position.lat);
		s->mutable_position()->set_lon(stop.second.position.lon);
		for (const auto& p : stop.second.known_distances) {
			KnownDistance* kd = s->add_known_distances();
			kd->set_stop(p.first);
			kd->set_distance(p.second);
		}
	}
	for (const auto& bus : bus_schedule.GetBuses()) {
		Bus* b = catalog.add_buses();
		b->set_name(bus.first);
		b->set_is_roundtrip(bus.second.is_roundtrip);
		for (const auto& stop : bus.second.stops) {
			b->add_stops(stop);
		}
	}
	std::ofstream out(_file, std::ios_base::binary);
	catalog.SerializeToOstream(&out);
	out.close();
}
