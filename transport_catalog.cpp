#include "transport_catalog.h"
#include "utils.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <optional>
#include <sstream>
#include <unordered_map>

using namespace std;

TransportCatalog::TransportCatalog(
    vector<Descriptions::InputQuery> data,
    const Json::Dict& routing_settings_json,
    const Json::Dict& render_settings_json,
    const Json::Dict& yellow_pages_json
) {
  auto stops_end = partition(begin(data), end(data), [](const auto& item) {
    return holds_alternative<Descriptions::Stop>(item);
  });

  Descriptions::StopsDict stops_dict;
  for (const auto& item : Range{begin(data), stops_end}) {
    const auto& stop = get<Descriptions::Stop>(item);
    stops_dict[stop.name] = &stop;
    stops_.insert({stop.name, {}});
  }

  Descriptions::BusesDict buses_dict;
  for (const auto& item : Range{stops_end, end(data)}) {
    const auto& bus = get<Descriptions::Bus>(item);

    buses_dict[bus.name] = &bus;
    buses_[bus.name] = Bus{
      bus.stops.size(),
      ComputeUniqueItemsCount(AsRange(bus.stops)),
      ComputeRoadRouteLength(bus.stops, stops_dict),
      ComputeGeoRouteDistance(bus.stops, stops_dict)
    };

    for (const string& stop_name : bus.stops) {
      stops_.at(stop_name).bus_names.insert(bus.name);
    }
  }
  router_ = make_unique<TransportRouter>(stops_dict, buses_dict, routing_settings_json);
  
  map_renderer_ = make_unique<MapRenderer>(stops_dict, buses_dict, render_settings_json);
  map_ = map_renderer_->Render();
  _yellow_pages = make_unique<YellowPages>(yellow_pages_json);
}

TransportCatalog::TransportCatalog(std::istream& input)
{
    TransportCatalogInfo catalog;
    catalog.ParseFromIstream(&input);
    for (int i = 0; i < catalog.stops_size(); ++i) {
        const auto& stop = catalog.stops(i);
        Stop s;
        for (int j = 0; j < stop.buses_size(); ++j) {
            s.bus_names.insert(stop.buses(j));
        }
        stops_[stop.name()] = std::move(s);
    }
    for (int i = 0; i < catalog.buses_size(); ++i) {
        const auto& bus = catalog.buses(i);
        Bus b;
        b.geo_route_length = bus.geo_route_length();
        b.road_route_length = bus.road_route_length();
        b.stop_count = bus.stop_count();
        b.unique_stop_count = bus.unique_stop_count();
        buses_[bus.name()] = std::move(b);
    }
    router_ = make_unique<TransportRouter>(catalog.router());
    map_renderer_ = make_unique<MapRenderer>(catalog.renderer());
    map_ = map_renderer_->Render();
    _yellow_pages = make_unique<YellowPages>(std::move(*catalog.mutable_database()));
}

const TransportCatalog::Stop* TransportCatalog::GetStop(const string& name) const {
  return GetValuePointer(stops_, name);
}

const TransportCatalog::Bus* TransportCatalog::GetBus(const string& name) const {
  return GetValuePointer(buses_, name);
}

optional<TransportRouter::RouteInfo> TransportCatalog::FindRoute(const string& stop_from, const string& stop_to) const {
  return router_->FindRoute(stop_from, stop_to);
}

std::vector<CompanyInfo> TransportCatalog::FindCompanies(const CompanyQuery& query) const
{
    return _yellow_pages->FindCompanies(query);
}

string TransportCatalog::RenderMap() const {
  ostringstream out;
  map_.Render(out);
  return out.str();
}

string TransportCatalog::RenderRoute(const TransportRouter::RouteInfo& route) const {
  ostringstream out;
  BuildRouteMap(route).Render(out);
  return out.str();
}

void TransportCatalog::Serialize(std::ostream& out) const
{
    TransportCatalogInfo catalog;
    for (const auto& stop : stops_) {
        auto* stop_info = catalog.add_stops();
        stop_info->set_name(stop.first);
        for (const auto& bus : stop.second.bus_names) {
            stop_info->add_buses(bus);
        }
    }
    for (const auto& bus : buses_) {
        auto* bus_info = catalog.add_buses();
        bus_info->set_name(bus.first);
        bus_info->set_stop_count(bus.second.stop_count);
        bus_info->set_unique_stop_count(bus.second.unique_stop_count);
        bus_info->set_road_route_length(bus.second.road_route_length);
        bus_info->set_geo_route_length(bus.second.geo_route_length);
    }
    
    router_->Serialize(*catalog.mutable_router());
    map_renderer_->Seriliaze(*catalog.mutable_renderer());
    _yellow_pages->Serialize(*catalog.mutable_database());
    catalog.SerializeToOstream(&out);
}

int TransportCatalog::ComputeRoadRouteLength(
    const vector<string>& stops,
    const Descriptions::StopsDict& stops_dict
) {
  int result = 0;
  for (size_t i = 1; i < stops.size(); ++i) {
    result += Descriptions::ComputeStopsDistance(*stops_dict.at(stops[i - 1]), *stops_dict.at(stops[i]));
  }
  return result;
}

double TransportCatalog::ComputeGeoRouteDistance(
    const vector<string>& stops,
    const Descriptions::StopsDict& stops_dict
) {
  double result = 0;
  for (size_t i = 1; i < stops.size(); ++i) {
    result += Sphere::Distance(
      stops_dict.at(stops[i - 1])->position, stops_dict.at(stops[i])->position
    );
  }
  return result;
}

Svg::Document TransportCatalog::BuildRouteMap(const TransportRouter::RouteInfo& route) const {
  return map_renderer_->RenderRoute(map_, route);
}
