#include "bus_schedule.h"

Catalog::BusShedule::BusShedule()
{
}

void Catalog::BusShedule::AddStop(const Models::StopInfo& stop_info)
{
    _stops[stop_info.name].position = stop_info.position;
    _stops[stop_info.name].name = stop_info.name;
    for (const auto& p : stop_info.known_distance) {
        _stops[stop_info.name].known_distances[p.first] = p.second;
    }
    _router_changed = true;
}

void Catalog::BusShedule::AddBus(const Models::BusInfo& bus_info)
{
    _buses[bus_info.name].stops = bus_info.stops;
    _buses[bus_info.name].is_roundtrip = bus_info.is_roundtrip;
    for (const auto& stop : bus_info.stops) {
        _stops[stop].buses.insert(bus_info.name);
    }
    _router_changed = true;
}

std::optional<Range<Catalog::BusShedule::BusStopsIterator>> Catalog::BusShedule::FindBusStops(const std::string& bus) const
{
    auto it = _buses.find(bus);
    if (it != _buses.end()) {
        return Range<BusStopsIterator>(it->second.stops.cbegin(), it->second.stops.cend());
    }
    return std::nullopt;
}

std::optional<Range<Catalog::BusShedule::StopBusesIterator>> Catalog::BusShedule::FindStopBuses(const std::string& stop)
{
    auto it = _stops.find(stop);
    if (it != _stops.end()) {
        return Range<StopBusesIterator>(it->second.buses.cbegin(), it->second.buses.cend());
    }
    return std::nullopt;
}

Models::Position Catalog::BusShedule::GetStopPosition(const std::string& stop) const
{
    auto it = _stops.find(stop);
    if (it != _stops.end()) {
        return it->second.position;
    }
    throw std::runtime_error("stop " + stop + " not found");
}

double Catalog::BusShedule::GetRouteDistance(const std::string& first, const std::string& second) const
{
    auto forward = TryFindDistance(first, second);
    if (forward) {
        return forward.value();
    }
    auto reverse = TryFindDistance(second, first);
    if (reverse) {
        return reverse.value();
    }

    throw std::runtime_error("stop " + first + " not found");
}

void Catalog::BusShedule::SetRouteParameters(double wait_time, double velocity)
{
    _router_changed = true;
    _wait_time = wait_time;
    _velocity = velocity;
}

Models::RouteInfo Catalog::BusShedule::FindOptimalWay(const std::string& from, const std::string& to)
{
    RebuildRouterIfNeeded();
    Models::RouteInfo result;
    Graph::VertexId from_vertex = _stop_to_vertex[from];
    Graph::VertexId to_vertex = _stop_to_vertex[to];
    auto route = _router->BuildRoute(from_vertex, to_vertex);
    if (route.has_value()) {
        result.found = true;
        result.total_time = route->weight;
        for (size_t idx = 0; idx < route->edge_count; ++idx) {
            Graph::EdgeId edge_id = _router->GetRouteEdge(route->id, idx);
            Models::EdgeInfo info = _edge_to_edge_info[edge_id];
            Models::RouteSegmentInfo segment;
            segment.bus = info.bus;
            segment.span_count = info.span_count;
            segment.stop_from = info.from;
            segment.trip_time = info.trip_time;
            segment.wait_time = _wait_time;
            segment.stops = info.stops;
            result.segments.push_back(std::move(segment));
        }
        _router->ReleaseRoute(route->id);
    }   
    return result;
}

Range<Catalog::BusShedule::StopsIterator> Catalog::BusShedule::GetStops() const
{
	return Range<Catalog::BusShedule::StopsIterator>(_stops.cbegin(), _stops.cend());
}

Range<Catalog::BusShedule::BusesIterator> Catalog::BusShedule::GetBuses() const
{
    return Range<Catalog::BusShedule::BusesIterator>(_buses.cbegin(), _buses.cend());
}

const Models::Bus& Catalog::BusShedule::GetBus(const std::string& bus) const
{
    return _buses.at(bus);
}

double Catalog::BusShedule::GetVelocityMetersPerMinute() const
{
    return (_velocity * 1000.00 / 60.00);
}

std::optional<double> Catalog::BusShedule::TryFindDistance(const std::string& first, const std::string& second) const
{
    auto it = _stops.find(first);
    if (it != _stops.end()) {
        auto it2 = it->second.known_distances.find(second);
        if (it2 != it->second.known_distances.end()) {
            return it2->second;
        }
    }
    return std::nullopt;
}

void Catalog::BusShedule::FillVertexes()
{
    _stop_to_vertex.clear();
    _stop_to_vertex.reserve(_stops.size());
    Graph::VertexId index = 0;
    for (const auto& p : _stops) {
        _stop_to_vertex[p.first] = index;
        ++index;
    }
}

void Catalog::BusShedule::FillEdges()
{
    _edge_to_edge_info.clear();
    double adj_velocity = GetVelocityMetersPerMinute();
    for (const auto& bus_pair : _buses)
    {
        const std::vector<std::string>& bus_stops = bus_pair.second.stops;
        for (size_t from_index = 0; from_index < bus_stops.size() - 1; ++from_index) {
            const std::string& from = bus_stops[from_index];
            Graph::VertexId from_idx = _stop_to_vertex[from];
            double distance = 0.0;
            std::vector<std::string_view> stops;
            for (size_t to_index = from_index + 1; to_index < bus_stops.size(); ++to_index) {
                const std::string& previous = bus_stops[to_index - 1];
                const std::string& to = bus_stops[to_index];
                stops.push_back(to);
                Graph::VertexId to_idx = _stop_to_vertex[to];
                distance += GetRouteDistance(previous, to);
                Models::EdgeInfo edge;
                edge.bus = bus_pair.first;
                edge.from = from;
                edge.stops = stops;
                edge.trip_time = distance / adj_velocity;
                edge.span_count = to_index - from_index;

                Graph::Edge<double> e;
                e.from = from_idx;
                e.to = to_idx;
                e.weight = edge.trip_time + _wait_time;
                Graph::EdgeId edge_id = _graph->AddEdge(e);

                _edge_to_edge_info[edge_id] = edge;
            }
        }
    }
}

void Catalog::BusShedule::BuildGraph()
{
    _graph = std::make_unique<Graph::DirectedWeightedGraph<double>>(_stops.size());
    FillVertexes();
    FillEdges();
}

void Catalog::BusShedule::BuildRouter()
{
    _router = std::make_unique<Graph::Router<double>>(*_graph);
}

void Catalog::BusShedule::RebuildRouterIfNeeded()
{
    if (!_router_changed) {
        return;
    }
    BuildGraph();
    BuildRouter();
    _router_changed = false;
}
