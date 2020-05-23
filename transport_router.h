#pragma once

#include "descriptions.h"
#include "graph.h"
#include "json.h"
#include "router.h"
#include "transport_catalog.pb.h"
#include "yellow_pages.h"

#include <memory>
#include <unordered_map>
#include <vector>
#include <iostream>

class TransportRouter {
private:
  using BusGraph = Graph::DirectedWeightedGraph<double>;
  using Router = Graph::Router<double>;
  using RouterData = serialization::Router;

public:
  TransportRouter(const Descriptions::StopsDict& stops_dict,
                  const Descriptions::BusesDict& buses_dict,
                  const Descriptions::CompaniesDict& companies_dict,
                  const Json::Dict& routing_settings_json);

  TransportRouter(const RouterData& router);

  struct RouteInfo {
    double total_time;

    struct BusItem {
      std::string bus_name;
      double time;
      size_t start_stop_idx;
      size_t finish_stop_idx;
      size_t span_count;
    };
    struct WaitItem {
      std::string stop_name;
      double time;
    };
    struct WalkToCompany {
        std::string stop_name;
        std::string company_name;
        std::string company_display_name;
        int company_id;
        double time;
    };

    struct WaitCompany {
        double time;
        std::string company_name;
    };

    using Item = std::variant<BusItem, WaitItem, WalkToCompany, WaitCompany>;
    std::vector<Item> items;
  };

  std::optional<RouteInfo> FindRoute(const std::string& stop_from, const std::string& stop_to) const;
  void ParseRoute(TransportRouter::RouteInfo& route_info, const TransportRouter::Router::RouteInfo& route) const;
  std::optional<RouteInfo> FindRoute(const std::string& stop_from, int company_id) const;
  void Serialize(RouterData& router);
private:
  struct RoutingSettings {
    int bus_wait_time;  // in minutes
    double bus_velocity;  // km/h
    double pedestrian_velocity;
  };

  static RoutingSettings MakeRoutingSettings(const Json::Dict& json);

  void FillGraphVertices(const Descriptions::StopsDict& stops_dict, const Descriptions::CompaniesDict& companies_dict);

  void FillGraphWithBuses(const Descriptions::StopsDict& stops_dict,
                          const Descriptions::BusesDict& buses_dict);

  struct StopVertexIds {
    Graph::VertexId in;
    Graph::VertexId out;
  };
  struct VertexInfo {
    std::string stop_name;
    int company_id;
  };

  struct BusEdgeInfo {
    std::string bus_name;
    size_t start_stop_idx;
    size_t finish_stop_idx;
  };

  struct WaitEdgeInfo {};

  struct WalkEdgeInfo {
      std::string stop_from;
      std::string company_name;
      std::string company_display_name;
      int company_id;
  };

  using EdgeInfo = std::variant<BusEdgeInfo, WaitEdgeInfo, WalkEdgeInfo>;

  RoutingSettings routing_settings_;
  BusGraph graph_;
  // TODO: Tell about this unique_ptr usage case
  std::unique_ptr<Router> router_;
  std::unordered_map<std::string, StopVertexIds> stops_vertex_ids_;
  std::vector<VertexInfo> vertices_info_;
  std::vector<EdgeInfo> edges_info_;
  std::unordered_map<int, Graph::VertexId> _companies_vertices;
};
