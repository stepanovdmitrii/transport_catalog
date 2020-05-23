#include "transport_router.h"

using namespace std;


TransportRouter::TransportRouter(const Descriptions::StopsDict& stops_dict,
                                 const Descriptions::BusesDict& buses_dict,
                                 const Descriptions::CompaniesDict& companies_dict,
                                 const Json::Dict& routing_settings_json)
    : routing_settings_(MakeRoutingSettings(routing_settings_json))
{
  const size_t vertex_count = stops_dict.size() * 2 + companies_dict.size();
  vertices_info_.resize(vertex_count);
  graph_ = BusGraph(vertex_count);

  FillGraphVertices(stops_dict, companies_dict);
  FillGraphWithBuses(stops_dict, buses_dict);

  router_ = std::make_unique<Router>(graph_);
}

TransportRouter::TransportRouter(const RouterData& router)
{
    routing_settings_.bus_velocity = router.bus_velocity();
    routing_settings_.bus_wait_time = router.bus_wait_time();
    routing_settings_.pedestrian_velocity = router.pedestrian_velocity();

    graph_ = BusGraph(router.graph().vertex_count(), router.graph().edges_size());
    for (int edge_idx = 0; edge_idx < router.graph().edges_size(); ++edge_idx) {
        const auto& edge = router.graph().edges(edge_idx);
        Graph::Edge<double> new_edge;
        new_edge.from = edge.from();
        new_edge.to = edge.to();
        new_edge.weight = edge.weight();
        graph_.AddEdge(new_edge);
    }

    Router::RoutesInternalData internal_data(router.route_impl().vectors_size());
    for (int idx = 0; idx < router.route_impl().vectors_size(); ++idx) {
        std::vector<std::optional<Router::RouteInternalData>> current(router.route_impl().vectors(idx).vector_data_size());
        for (int idx2 = 0; idx2 < router.route_impl().vectors(idx).vector_data_size(); ++idx2) {
            const auto& data = router.route_impl().vectors(idx).vector_data(idx2);
            if (data.has_value()) {
                Router::RouteInternalData source_data;
                source_data.weight = data.weight();
                if (data.has_prev_edge()) {
                    source_data.prev_edge = data.prev_edge_id();
                }
                else {
                    source_data.prev_edge = nullopt;
                }
                current[idx2] = source_data;
            }
            else {
                current[idx2] = nullopt;
            }
        }
        internal_data[idx] = std::move(current);
    }

    for (int idx = 0; idx < router.stop_vertex_ids_size(); ++idx) {
        const auto& stop_vertex_ids = router.stop_vertex_ids(idx);
        StopVertexIds ids;
        ids.in = stop_vertex_ids.in();
        ids.out = stop_vertex_ids.out();
        stops_vertex_ids_[stop_vertex_ids.name()] = ids;
    }

    vertices_info_.reserve(router.vertex_infos_size());
    for (int idx = 0; idx < router.vertex_infos_size(); ++idx) {
        const auto& vertex_info = router.vertex_infos(idx);
        VertexInfo info;
        info.stop_name = vertex_info.name();
        vertices_info_.push_back(info);
    }

    edges_info_.reserve(router.edge_infos_size());
    for (int idx = 0; idx < router.edge_infos_size(); ++idx) {
        const auto& edge_info = router.edge_infos(idx);
        if (edge_info.type() == serialization::EdgeInfoType::BUS) {
            BusEdgeInfo info;
            info.bus_name = edge_info.bus_name();
            info.start_stop_idx = edge_info.bus_start_stop_idx();
            info.finish_stop_idx = edge_info.bus_finish_stop_idx();
            EdgeInfo new_edge_info(info);
            edges_info_.push_back(new_edge_info);
        }
        else if (edge_info.type() == serialization::EdgeInfoType::WAIT) {
            WaitEdgeInfo info;
            EdgeInfo new_edge_info(info);
            edges_info_.push_back(new_edge_info);
        }
        else if (edge_info.type() == serialization::EdgeInfoType::WALK) {
            WalkEdgeInfo info;
            info.stop_from = edge_info.stop_from();
            info.company_id = edge_info.company_id();
            info.company_display_name = edge_info.company_display_name();
            info.company_name = edge_info.company_name();
            edges_info_.push_back(info);
        }
    }

    for (const auto& pair : router.companies_verticies()) {
        _companies_vertices[pair.first] = pair.second;
    }

    router_ = std::make_unique<Router>(graph_, std::move(internal_data));
}

TransportRouter::RoutingSettings TransportRouter::MakeRoutingSettings(const Json::Dict& json) {
  return {
      json.at("bus_wait_time").AsInt(),
      json.at("bus_velocity").AsDouble(),
      json.at("pedestrian_velocity").AsDouble()
  };
}

void TransportRouter::FillGraphVertices(const Descriptions::StopsDict& stops_dict, const Descriptions::CompaniesDict& companies_dict) {
    Graph::VertexId vertex_id = 0;

    for (const auto& [stop_name, _] : stops_dict) {
        auto& vertex_ids = stops_vertex_ids_[stop_name];
        vertex_ids.in = vertex_id++;
        vertex_ids.out = vertex_id++;
        vertices_info_[vertex_ids.in] = { stop_name, -1 };
        vertices_info_[vertex_ids.out] = { stop_name, -1 };

        edges_info_.push_back(WaitEdgeInfo{});
        const Graph::EdgeId edge_id = graph_.AddEdge({
            vertex_ids.out,
            vertex_ids.in,
            static_cast<double>(routing_settings_.bus_wait_time)
            });
        assert(edge_id == edges_info_.size() - 1);
    }

    for (const auto& company : companies_dict) {
        VertexInfo vertex_info;
        vertex_info.company_id = company.first;
        _companies_vertices[company.first] = vertex_id;
        vertices_info_[vertex_id] = std::move(vertex_info);
        for (const auto& stop : company.second.nearby_stops) {
            const auto stop_vertex_id = stops_vertex_ids_[stop.first].out;
            edges_info_.push_back(WalkEdgeInfo{ stop.first, company.second.name, company.second.display_name, company.first });
            const Graph::EdgeId edge_id = graph_.AddEdge({
                stop_vertex_id,
                vertex_id,
                 stop.second * 1.0 / (routing_settings_.pedestrian_velocity * 1000.0 / 60)
                });
            assert(edge_id == edges_info_.size() - 1);
        }
        vertex_id++;
    }
    assert(vertex_id == graph_.GetVertexCount());
}

void TransportRouter::FillGraphWithBuses(const Descriptions::StopsDict& stops_dict,
    const Descriptions::BusesDict& buses_dict) {
    for (const auto& [_, bus_item] : buses_dict) {
        const auto& bus = *bus_item;
        const size_t stop_count = bus.stops.size();
        if (stop_count <= 1) {
            continue;
        }
        auto compute_distance_from = [&stops_dict, &bus](size_t lhs_idx) {
            return Descriptions::ComputeStopsDistance(*stops_dict.at(bus.stops[lhs_idx]), *stops_dict.at(bus.stops[lhs_idx + 1]));
        };
        for (size_t start_stop_idx = 0; start_stop_idx + 1 < stop_count; ++start_stop_idx) {
            const Graph::VertexId start_vertex = stops_vertex_ids_[bus.stops[start_stop_idx]].in;
            int total_distance = 0;
            for (size_t finish_stop_idx = start_stop_idx + 1; finish_stop_idx < stop_count; ++finish_stop_idx) {
                total_distance += compute_distance_from(finish_stop_idx - 1);
                BusEdgeInfo edge_info;
                edge_info.bus_name = bus.name;
                edge_info.start_stop_idx = start_stop_idx;
                edge_info.finish_stop_idx = finish_stop_idx;

                edges_info_.push_back(std::move(edge_info));
                const Graph::EdgeId edge_id = graph_.AddEdge({
                    start_vertex,
                    stops_vertex_ids_[bus.stops[finish_stop_idx]].out,
                    total_distance * 1.0 / (routing_settings_.bus_velocity * 1000.0 / 60)  // m / (km/h * 1000 / 60) = min
                    });
                assert(edge_id == edges_info_.size() - 1);
            }
        }
    }
}

optional<TransportRouter::RouteInfo> TransportRouter::FindRoute(const string& stop_from, const string& stop_to) const {
    const Graph::VertexId vertex_from = stops_vertex_ids_.at(stop_from).out;
    const Graph::VertexId vertex_to = stops_vertex_ids_.at(stop_to).out;
    const auto route = router_->BuildRoute(vertex_from, vertex_to);
    if (!route) {
        return nullopt;
    }
    RouteInfo route_info;
    ParseRoute(route_info, route.value());

    // Releasing in destructor of some proxy object would be better,
    // but we do not expect exceptions in normal workflow
    router_->ReleaseRoute(route->id);
    return route_info;
}

void TransportRouter::ParseRoute(TransportRouter::RouteInfo& route_info, const TransportRouter::Router::RouteInfo& route) const
{
    route_info.total_time = route.weight;
    route_info.items.reserve(route.edge_count + 1);
    for (size_t edge_idx = 0; edge_idx < route.edge_count; ++edge_idx) {
        const Graph::EdgeId edge_id = router_->GetRouteEdge(route.id, edge_idx);
        const auto& edge = graph_.GetEdge(edge_id);
        const auto& edge_info = edges_info_[edge_id];
        if (holds_alternative<BusEdgeInfo>(edge_info)) {
            const BusEdgeInfo& bus_edge_info = get<BusEdgeInfo>(edge_info);
            RouteInfo::BusItem bus_item;
            bus_item.bus_name = bus_edge_info.bus_name;
            bus_item.time = edge.weight;
            bus_item.start_stop_idx = bus_edge_info.start_stop_idx;
            bus_item.finish_stop_idx = bus_edge_info.finish_stop_idx;
            bus_item.span_count = bus_edge_info.finish_stop_idx - bus_edge_info.start_stop_idx;
            route_info.items.push_back(std::move(bus_item));
        }
        else if(holds_alternative<WaitEdgeInfo>(edge_info)) {
            const Graph::VertexId vertex_id = edge.from;
            RouteInfo::WaitItem wait_item;
            wait_item.stop_name = vertices_info_[vertex_id].stop_name;
            wait_item.time = edge.weight;
            route_info.items.push_back(std::move(wait_item));
        }
        else if (holds_alternative<WalkEdgeInfo>(edge_info)) {
            const WalkEdgeInfo& walk_edge_info = get<WalkEdgeInfo>(edge_info);
            RouteInfo::WalkToCompany walk_item;
            walk_item.stop_name = walk_edge_info.stop_from;
            walk_item.time = edge.weight;
            walk_item.company_id = walk_edge_info.company_id;
            walk_item.company_name = walk_edge_info.company_name;
            walk_item.company_display_name = walk_edge_info.company_display_name;
            route_info.items.push_back(std::move(walk_item));
        }
    }
}

std::optional<TransportRouter::RouteInfo> TransportRouter::FindRoute(const std::string& stop_from, int company_id) const
{
    const Graph::VertexId vertex_from = stops_vertex_ids_.at(stop_from).out;
    const Graph::VertexId vertex_to = _companies_vertices.at(company_id);
    const auto route = router_->BuildRoute(vertex_from, vertex_to);
    if (!route) {
        return nullopt;
    }
    RouteInfo route_info;
    ParseRoute(route_info, route.value());
    router_->ReleaseRoute(route->id);
    return route_info;
}

void TransportRouter::Serialize(RouterData& router)
{
    router.set_bus_velocity(routing_settings_.bus_velocity);
    router.set_bus_wait_time(routing_settings_.bus_wait_time);
    router.set_pedestrian_velocity(routing_settings_.pedestrian_velocity);

    auto* graph = router.mutable_graph();
    graph->set_vertex_count(graph_.GetVertexCount());
    for (size_t edge_idx = 0; edge_idx < graph_.GetEdgeCount(); ++edge_idx) {
        auto* edge_data = graph->add_edges();
        const auto& source_edge = graph_.GetEdge(edge_idx);
        edge_data->set_from(source_edge.from);
        edge_data->set_to(source_edge.to);
        edge_data->set_weight(source_edge.weight);
    }

    auto* router_impl = router.mutable_route_impl();
    for (const auto& v : router_->routes_internal_data_) {
        auto* vect = router_impl->add_vectors();
        for (const auto& d : v) {
            auto* data = vect->add_vector_data();
            data->set_has_value(d.has_value());
            if (d.has_value()) {
                data->set_weight(d.value().weight);
                data->set_has_prev_edge(d.value().prev_edge.has_value());
                if (d.value().prev_edge.has_value()) {
                    data->set_prev_edge_id(d.value().prev_edge.value());
                }
            }
        }
    }

    for (const auto& stop_vertex_id : stops_vertex_ids_) {
        auto* stops_vertex_id_data = router.add_stop_vertex_ids();
        stops_vertex_id_data->set_in(stop_vertex_id.second.in);
        stops_vertex_id_data->set_out(stop_vertex_id.second.out);
        stops_vertex_id_data->set_name(stop_vertex_id.first);
    }

    for (const auto& vertex_info : vertices_info_) {
        auto* vertex_info_data = router.add_vertex_infos();
        vertex_info_data->set_name(vertex_info.stop_name);
        vertex_info_data->set_company_id(vertex_info.company_id);
    }

    for (const auto& edge_info : edges_info_) {
        auto* edge_info_data = router.add_edge_infos();
        if (edge_info.index() == 0) { //bus
            edge_info_data->set_type(serialization::EdgeInfoType::BUS);
            edge_info_data->set_bus_name(std::get<BusEdgeInfo>(edge_info).bus_name);
            edge_info_data->set_bus_start_stop_idx(std::get<BusEdgeInfo>(edge_info).start_stop_idx);
            edge_info_data->set_bus_finish_stop_idx(std::get<BusEdgeInfo>(edge_info).finish_stop_idx);
        }
        else if (edge_info.index() == 1) { //wait
            edge_info_data->set_type(serialization::EdgeInfoType::WAIT);
        }
        else if (edge_info.index() == 2) { //walk
            edge_info_data->set_type(serialization::EdgeInfoType::WALK);
            edge_info_data->set_stop_from(std::get<WalkEdgeInfo>(edge_info).stop_from);
            edge_info_data->set_company_id(std::get<WalkEdgeInfo>(edge_info).company_id);
            edge_info_data->set_company_name(std::get<WalkEdgeInfo>(edge_info).company_name);
            edge_info_data->set_company_display_name(std::get<WalkEdgeInfo>(edge_info).company_display_name);
        }
    }

    for (const auto& pair : _companies_vertices) {
        (*router.mutable_companies_verticies())[pair.first] = pair.second;
    }
}
