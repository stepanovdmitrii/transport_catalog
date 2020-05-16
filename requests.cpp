#include "requests.h"
#include "transport_router.h"

#include <vector>

using namespace std;

namespace Requests {

  Json::Dict Stop::Process(const TransportCatalog& db) const {
    const auto* stop = db.GetStop(name);
    Json::Dict dict;
    if (!stop) {
      dict["error_message"] = Json::Node("not found"s);
    } else {
      Json::Array bus_nodes;
      bus_nodes.reserve(stop->bus_names.size());
      for (const auto& bus_name : stop->bus_names) {
        bus_nodes.emplace_back(bus_name);
      }
      dict["buses"] = Json::Node(move(bus_nodes));
    }
    return dict;
  }

  Json::Dict Bus::Process(const TransportCatalog& db) const {
    const auto* bus = db.GetBus(name);
    Json::Dict dict;
    if (!bus) {
      dict["error_message"] = Json::Node("not found"s);
    } else {
      dict = {
          {"stop_count", Json::Node(static_cast<int>(bus->stop_count))},
          {"unique_stop_count", Json::Node(static_cast<int>(bus->unique_stop_count))},
          {"route_length", Json::Node(bus->road_route_length)},
          {"curvature", Json::Node(bus->road_route_length / bus->geo_route_length)},
      };
    }
    return dict;
  }

  struct RouteItemResponseBuilder {
    Json::Dict operator()(const TransportRouter::RouteInfo::BusItem& bus_item) const {
      return Json::Dict{
          {"type", Json::Node("Bus"s)},
          {"bus", Json::Node(bus_item.bus_name)},
          {"time", Json::Node(bus_item.time)},
          {"span_count", Json::Node(static_cast<int>(bus_item.span_count))}
      };
    }
    Json::Dict operator()(const TransportRouter::RouteInfo::WaitItem& wait_item) const {
      return Json::Dict{
          {"type", Json::Node("Wait"s)},
          {"stop_name", Json::Node(wait_item.stop_name)},
          {"time", Json::Node(wait_item.time)},
      };
    }
  };

  Json::Dict Route::Process(const TransportCatalog& db) const {
    Json::Dict dict;
    const auto route = db.FindRoute(stop_from, stop_to);
    if (!route) {
      dict["error_message"] = Json::Node("not found"s);
    } else {
      dict["total_time"] = Json::Node(route->total_time);
      Json::Array items;
      items.reserve(route->items.size());
      for (const auto& item : route->items) {
        items.push_back(visit(RouteItemResponseBuilder{}, item));
      }

      dict["items"] = move(items);

      dict["map"] = Json::Node(db.RenderRoute(*route));
    }

    return dict;
  }

  Json::Dict Map::Process(const TransportCatalog& db) const {
    return Json::Dict{
        {"map", Json::Node(db.RenderMap())},
    };
  }

  variant<Stop, Bus, Route, Map, Companies> Read(const Json::Dict& attrs) {
    const string& type = attrs.at("type").AsString();
    if (type == "Bus") {
      return Bus{attrs.at("name").AsString()};
    } else if (type == "Stop") {
      return Stop{attrs.at("name").AsString()};
    } else if (type == "Route") {
      return Route{attrs.at("from").AsString(), attrs.at("to").AsString()};
    } else if (type == "FindCompanies") {
        return Companies::Create(attrs);
    }
    else {
      return Map{};
    }
  }

  Json::Array ProcessAll(const TransportCatalog& db, const Json::Array& requests) {
    Json::Array responses;
    responses.reserve(requests.size());
    for (const Json::Node& request_node : requests) {
      Json::Dict dict = visit([&db](const auto& request) {
                                return request.Process(db);
                              },
                              Requests::Read(request_node.AsMap()));
      dict["request_id"] = Json::Node(request_node.AsMap().at("id").AsInt());
      responses.push_back(Json::Node(dict));
    }
    return responses;
  }

  Json::Dict Companies::Process(const TransportCatalog& db) const
  {
      Json::Dict dict;
      Json::Array companies;
     
      bool name_match = false;
      bool url_match = false;
      bool rubric_match = false;
      bool phone_match = false;

      for (const auto& source_company : db.GetDatabase().companies()) {

          name_match = names.empty();
          if (!name_match && source_company.names_size() > 0) {
              for (const auto& source_name : source_company.names()) {
                  if (names.find(source_name.value()) != names.end()) {
                      name_match = true;
                      break;
                  }
              }
          }

          if (!name_match) {
              continue;
          }

          url_match = urls.empty();
          if (!url_match && source_company.urls_size() > 0) {
              for (const auto& source_url : source_company.urls()) {
                  if (urls.find(source_url.value()) != urls.end()) {
                      url_match = true;
                      break;
                  }
              }
          }

          if (!url_match) {
              continue;
          }

          rubric_match = rubrics.empty();
          if (!rubric_match && source_company.rubrics_size() > 0) {
              for (const auto& source_rubric_id : source_company.rubrics()) {
                  const auto& source_rubric = db.GetDatabase().rubrics().at(source_rubric_id);
                  if (rubrics.find(source_rubric.name()) != rubrics.end()) {
                      rubric_match = true;
                      break;
                  }
              }
          }

          if (!rubric_match) {
              continue;
          }

          phone_match = phones.empty();
          if (!phone_match && source_company.phones_size() > 0) {
              for (const auto& source_phone : source_company.phones()) {
                  if (phone_match) {
                      break;
                  }
                  for (const auto& target_phone : phones) {
                      phone_match = target_phone.DoesPhoneMatch(source_phone);
                      if (phone_match) {
                          break;
                      }
                  }

              }
          }

          if (!phone_match) {
              continue;
          }

          for (const auto& name : source_company.names()) {
              if (name.type() == serialization::Name_Type::Name_Type_MAIN) {
                  companies.push_back(Json::Node(name.value()));
                  break;
              }
          }
      }

      dict["companies"] = companies;
      return dict;
  }

  Companies Companies::Create(const Json::Dict& attrs)
  {
      Companies result;
      auto names = attrs.find("names");
      if (names != attrs.end()) {
          for (const auto& name : names->second.AsArray()) {
              result.names.insert(name.AsString());
          }
      }

      auto urls = attrs.find("urls");
      if (urls != attrs.end()) {
          for (const auto& url : urls->second.AsArray()) {
              result.urls.insert(url.AsString());
          }
      }

      auto rubrics = attrs.find("rubrics");
      if (rubrics != attrs.end()) {
          for (const auto& rubric : rubrics->second.AsArray()) {
              result.rubrics.insert(rubric.AsString());
          }
      }

      auto phones = attrs.find("phones");
      if (phones != attrs.end()) {
          for (const auto& phoneNode : phones->second.AsArray()) {
              const auto& phone = phoneNode.AsMap();
              PhoneQuery templ;
              auto type = phone.find("type");
              if (type != phone.end()) {
                  templ.type = type->second.AsString();
              }
              auto country_code = phone.find("country_code");
              if (country_code != phone.end()) {
                  templ.country_code = country_code->second.AsString();
              }
              auto local_code = phone.find("local_code");
              if (local_code != phone.end()) {
                  templ.local_code = local_code->second.AsString();
              }
              auto number = phone.find("number");
              if (number != phone.end()) {
                  templ.number = number->second.AsString();
              }
              auto extension = phone.find("extension");
              if (extension != phone.end()) {
                  templ.extension = extension->second.AsString();
              }
              result.phones.push_back(std::move(templ));
          }
      }
      return std::move(result);
  }

  serialization::Phone_Type PhoneQuery::phone_type() const
  {
      if (type == "PHONE")
          return serialization::Phone_Type::Phone_Type_PHONE;
      return serialization::Phone_Type::Phone_Type_FAX;
  }

}
