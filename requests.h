#pragma once

#include "json.h"
#include "transport_catalog.h"
#include "transport_catalog.pb.h"

#include <string>
#include <variant>


namespace Requests {
  struct Stop {
    std::string name;

    Json::Dict Process(const TransportCatalog& db) const;
  };

  struct Bus {
    std::string name;

    Json::Dict Process(const TransportCatalog& db) const;
  };

  struct Route {
    std::string stop_from;
    std::string stop_to;

    Json::Dict Process(const TransportCatalog& db) const;
  };

  struct Map {
    Json::Dict Process(const TransportCatalog& db) const;
  };

  struct PhoneQuery {
      std::string type;
      std::string country_code;
      std::string local_code;
      std::string number;
      std::string extension;

      bool DoesPhoneMatch(const serialization::Phone& object) const {
          if (!extension.empty() && extension != object.extension()) {
              return false;
          }
          if (!type.empty() && phone_type() != object.type()) {
              return false;
          }
          if (!country_code.empty() && country_code != object.country_code()) {
              return false;
          }
          if (
              (!local_code.empty() || !country_code.empty())
              && local_code != object.local_code()
              ) {
              return false;
          }
          return number == object.number();
      }

  private:
      serialization::Phone_Type phone_type() const;
  };

  struct Companies {
      std::unordered_set<std::string> names;
      std::unordered_set<std::string> urls;
      std::unordered_set<std::string> rubrics;
      std::vector<PhoneQuery> phones;

      Json::Dict Process(const TransportCatalog& db) const;

      static Companies Create(const Json::Dict& attrs);
  };

  std::variant<Stop, Bus, Route, Map, Companies> Read(const Json::Dict& attrs);

  Json::Array ProcessAll(const TransportCatalog& db, const Json::Array& requests);
}
