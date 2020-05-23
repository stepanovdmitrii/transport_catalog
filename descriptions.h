#pragma once

#include "json.h"
#include "sphere.h"
#include "transport_catalog.pb.h"

#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>


namespace Descriptions {
  struct Stop {
    std::string name;
    Sphere::Point position;
    std::unordered_map<std::string, int> distances;

    static Stop ParseFrom(const Json::Dict& attrs);
  };

  int ComputeStopsDistance(const Stop& lhs, const Stop& rhs);

  struct Bus {
    std::string name;
    std::vector<std::string> stops;
    std::vector<std::string> endpoints;

    static Bus ParseFrom(const Json::Dict& attrs);
  };

  struct Company {
      int id;
      std::string name;
      std::string display_name;
      std::unordered_map<std::string, uint32_t> nearby_stops;
      double lon;
      double lat;
  };

  struct DateTime {
      int32_t day_of_week;
      int32_t hour;
      int32_t minutes;
  };

  using InputQuery = std::variant<Stop, Bus>;

  std::vector<InputQuery> ReadDescriptions(const Json::Array& nodes);

  template <typename Object>
  using Dict = std::map<std::string, const Object*>;

  using StopsDict = Dict<Stop>;
  using BusesDict = Dict<Bus>;
  using CompaniesDict = std::unordered_map<int, Company>;
}
