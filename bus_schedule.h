#pragma once

#include "stop_info.h"
#include "bus_info.h"
#include "range.h"
#include "route_info.h"
#include "stop.h"
#include "graph.h"
#include "router.h"
#include "edge_info.h"
#include "position.h"
#include "bus.h"

#include <vector>
#include <string>
#include <set>
#include <optional>
#include <unordered_map>
#include <memory>
#include <exception>
#include <algorithm>
#include <map>
#include <iostream>

namespace Catalog {
    class BusShedule {
    public:
        using BusStopsIterator = std::vector<std::string>::const_iterator;
        using StopBusesIterator = std::set<std::string>::const_iterator;
        using StopsIterator = std::map<std::string, Models::Stop>::const_iterator;
        using BusesIterator = std::map<std::string, Models::Bus>::const_iterator;

        BusShedule(const BusShedule&) = delete;
        BusShedule(BusShedule&&) = delete;
        BusShedule();

        virtual void AddStop(const Models::StopInfo& stop_info);
        virtual void AddBus(const Models::BusInfo& bus_info);

        std::optional<Range<BusStopsIterator>> FindBusStops(const std::string& bus) const;
        std::optional<Range<StopBusesIterator>> FindStopBuses(const std::string& stop);
        Models::Position GetStopPosition(const std::string& stop) const;
        double GetRouteDistance(const std::string& first, const std::string& second) const;
        void SetRouteParameters(double wait_time, double velocity);
        Models::RouteInfo FindOptimalWay(const std::string& from, const std::string& to);
        Range<StopsIterator> GetStops() const;
        Range<BusesIterator> GetBuses() const;
    private:
        std::map<std::string, Models::Stop> _stops;
        std::map<std::string, Models::Bus> _buses;

        bool _router_changed = true;
        std::unique_ptr<Graph::DirectedWeightedGraph<double>> _graph;
        std::unique_ptr<Graph::Router<double>> _router;
        std::unordered_map<std::string, Graph::VertexId> _stop_to_vertex;
        std::unordered_map<Graph::EdgeId, Models::EdgeInfo> _edge_to_edge_info;

        double _wait_time = 1.0;
        double _velocity = 1.0;

        double GetVelocityMetersPerMinute() const;
        std::optional<double> TryFindDistance(const std::string& first, const std::string& second) const;
        void FillVertexes();
        void FillEdges();
        void BuildGraph();
        void BuildRouter();
        void RebuildRouterIfNeeded();
    };
}