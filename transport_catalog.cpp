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
  ParseDatabase(yellow_pages_json);
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
    database_ = std::move(*catalog.mutable_database());
}

const TransportCatalog::Stop* TransportCatalog::GetStop(const string& name) const {
  return GetValuePointer(stops_, name);
}

const TransportCatalog::Bus* TransportCatalog::GetBus(const string& name) const {
  return GetValuePointer(buses_, name);
}

const serialization::Database& TransportCatalog::GetDatabase() const
{
    return database_;
}

optional<TransportRouter::RouteInfo> TransportCatalog::FindRoute(const string& stop_from, const string& stop_to) const {
  return router_->FindRoute(stop_from, stop_to);
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
    *(catalog.mutable_database()) = std::move(database_);
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

void TransportCatalog::ParseDatabase(const Json::Dict& yellow_pages_json)
{
    ParseRubrics(yellow_pages_json);
    ParseCompanies(yellow_pages_json);
}

void TransportCatalog::ParseCompanies(const Json::Dict& yellow_pages_json)
{
    const auto& company_arr = yellow_pages_json.at("companies").AsArray();
    for (const auto& company : company_arr) {
        serialization::Company* comp = database_.add_companies();;
        const auto& company_dict = company.AsMap();
        //ParseCompanyAddress(*comp, company_dict);
        ParseCompanyNames(*comp, company_dict);
        ParseCompanyPhones(*comp, company_dict);
        ParseCompanyUrls(*comp, company_dict);
        ParseCompanyRubrics(*comp, company_dict);
        //working_time
        //ParseCompanyNearbyStops(*comp, company_dict);
    }
}

void TransportCatalog::ParseRubrics(const Json::Dict& yellow_pages_json)
{
    auto& rubrics = yellow_pages_json.at("rubrics").AsMap();
    for (const auto& rubric : rubrics)
    {
        uint64_t key;
        std::istringstream iss(rubric.first);
        iss >> key;

        serialization::Rubric r;
        const auto& rubric_dict = rubric.second.AsMap();
        r.set_name(rubric_dict.at("name").AsString());
        //if (rubric_dict.find("keywords") != rubric_dict.end()) {
        //    const auto& keywords_arr = rubric_dict.at("keywords").AsArray();
        //    for (const auto& keyword : keywords_arr) {
        //        r.add_keywords(keyword.AsString());
        //    }
        //}
        (*database_.mutable_rubrics())[key] = std::move(r);
    }
}

void TransportCatalog::ParseCompanyNearbyStops(serialization::Company& comp, const Json::Dict& company_dict)
{
    const auto& nearby_stops = company_dict.find("nearby_stops");
    if (nearby_stops != company_dict.end()) {
        for (const auto& nearby_stop : nearby_stops->second.AsArray()) {
            serialization::NearbyStop* nearby_stop_value = comp.add_nearby_stops();
            const auto& nearby_stop_dict = nearby_stop.AsMap();

            const auto& nearby_stop_name = nearby_stop_dict.find("name");
            if (nearby_stop_name != nearby_stop_dict.end()) {
                nearby_stop_value->set_name(nearby_stop_name->second.AsString());
            }

            const auto& nearby_stop_meters = nearby_stop_dict.find("meters");
            if (nearby_stop_meters != nearby_stop_dict.end()) {
                nearby_stop_value->set_meters(nearby_stop_meters->second.AsInt());
            }
        }
    }
}

void TransportCatalog::ParseCompanyRubrics(serialization::Company& comp, const Json::Dict& company_dict)
{
    const auto& comp_rubrics = company_dict.find("rubrics");
    if (comp_rubrics != company_dict.end()) {
        for (const auto& rubric_node : comp_rubrics->second.AsArray()) {
            comp.add_rubrics(rubric_node.AsInt());
        }
    }
}

void TransportCatalog::ParseCompanyUrls(serialization::Company& comp, const Json::Dict& company_dict)
{
    const auto& urls = company_dict.find("urls");
    if (urls != company_dict.end()) {
        const auto& urls_arr = urls->second.AsArray();
        for (const auto& url : urls_arr) {
            serialization::Url* url_value = comp.add_urls();
            url_value->set_value(url.AsMap().at("value").AsString());
        }
    }
}

void TransportCatalog::ParseCompanyAddress(serialization::Company& comp, const Json::Dict& company_dict)
{
    serialization::Address* address = comp.mutable_address();
    const auto& address_node = company_dict.find("address");
    if (address_node != company_dict.end()) {
        const auto& address_dict = company_dict.at("address").AsMap();

        const auto& formatted = address_dict.find("formatted");
        if (formatted != address_dict.end()) {
            address->set_formatted(formatted->second.AsString());
        }

        const auto& components = address_dict.find("components");
        if (components != address_dict.end()) {
            const auto& components_arr = components->second.AsArray();
            for (const auto& component : components_arr) {
                serialization::AddressComponent* component_value = address->add_components();
                const auto& component_dict = component.AsMap();
                component_value->set_value(component_dict.at("value").AsString());
            }
        }

        const auto& coords = address_dict.find("coords");
        if (coords != address_dict.end()) {
            const auto& coords_dict = coords->second.AsMap();
            std::stringstream lat_str(coords_dict.at("lat").AsString());
            std::stringstream lon_str(coords_dict.at("lon").AsString());
            double lat;
            double lon;
            lat_str >> lat;
            lon_str >> lon;
            address->mutable_coords()->set_lat(lat);
            address->mutable_coords()->set_lon(lon);
        }

        const auto& comment = address_dict.find("comment");
        if (comment != address_dict.end()) {
            address->set_comment(comment->second.AsString());
        }
    }
}

void TransportCatalog::ParseCompanyNames(serialization::Company& comp, const Json::Dict& company_dict)
{
    const auto& names_arr = company_dict.at("names").AsArray();
    for (const auto& name : names_arr) {
        const auto& name_dict = name.AsMap();
        serialization::Name* name_value = comp.add_names();;
        name_value->set_value(name_dict.at("value").AsString());
        const auto& type_node = name_dict.find("type");
        if (type_node == name_dict.end()) {
            name_value->set_type(serialization::Name_Type::Name_Type_MAIN);
        }
        else {
            if (type_node->second.AsString() == "MAIN") {
                name_value->set_type(serialization::Name_Type::Name_Type_MAIN);
            }
            else if (type_node->second.AsString() == "SHORT") {
                name_value->set_type(serialization::Name_Type::Name_Type_SHORT);
            }
            else {
                name_value->set_type(serialization::Name_Type::Name_Type_SYNONYM);
            }
        }

    }
}

void TransportCatalog::ParseCompanyPhones(serialization::Company& comp, const Json::Dict& company_dict)
{
    const auto& phones = company_dict.find("phones");
    if (phones != company_dict.end()) {
        for (const auto& phone_node : phones->second.AsArray()) {
            serialization::Phone* phone = comp.add_phones();
            const auto& phone_dict = phone_node.AsMap();

            const auto& formatted = phone_dict.find("formatted");
            if (formatted != phone_dict.end()) {
                phone->set_formatted(formatted->second.AsString());
            }

            const auto& type = phone_dict.find("type");
            if (type != phone_dict.end()) {
                if (type->second.AsString() == "PHONE") {
                    phone->set_type(serialization::Phone_Type::Phone_Type_PHONE);
                }
                else if (type->second.AsString() == "FAX") {
                    phone->set_type(serialization::Phone_Type::Phone_Type_FAX);
                }
            }
            else {
                phone->set_type(serialization::Phone_Type::Phone_Type_PHONE);
            }

            const auto& country_code = phone_dict.find("country_code");
            if (country_code != phone_dict.end()) {
                phone->set_country_code(country_code->second.AsString());
            }

            const auto& local_code = phone_dict.find("local_code");
            if (local_code != phone_dict.end()) {
                phone->set_local_code(local_code->second.AsString());
            }

            const auto& number = phone_dict.find("number");
            if (number != phone_dict.end()) {
                phone->set_number(number->second.AsString());
            }

            const auto& extension = phone_dict.find("extension");
            if (extension != phone_dict.end()) {
                phone->set_extension(extension->second.AsString());
            }

            const auto& description = phone_dict.find("description");
            if (description != phone_dict.end()) {
                phone->set_description(description->second.AsString());
            }
        }


    }
}
