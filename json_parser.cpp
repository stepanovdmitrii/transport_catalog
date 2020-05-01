#include "json_parser.h"

IO::JsonParser::JsonParser(std::istream& input): _input(input)
{
}

void IO::JsonParser::Parse()
{
    std::unique_ptr<Json::Document> document = std::make_unique<Json::Document>(Json::Load(_input));
    ParseRouteSettings(*document);
    ParseBaseRequests(*document);
    ParseStatRequests(*document);
    ParseRenderSettings(*document);
}

void IO::JsonParser::ParseRouteSettings(Json::Document& doc)
{
    auto root_map = doc.GetRoot().AsMap();
    const auto& routing_settings = root_map.at("routing_settings").AsMap();
    double wait_time = routing_settings.at("bus_wait_time").AsDouble();
    double velocity = routing_settings.at("bus_velocity").AsDouble();
    _update.push_back(std::move(std::make_shared<Operations::SetRouteParameters>(wait_time, velocity)));
}

void IO::JsonParser::ParseBaseRequests(Json::Document& doc)
{
    auto root_map = doc.GetRoot().AsMap();
    const auto& update_requests = root_map.at("base_requests").AsArray();
    for (const auto& r : update_requests) {
        const auto node = r.AsMap();
        std::string type = node.at("type").AsString();
        if (type == "Stop") {
            _update.push_back(std::move(ParseAddStop(node)));
        }
        else if (type == "Bus") {
            _update.push_back(std::move(ParseAddBus(node)));
        }
        else {
            throw std::runtime_error("unknown update operation type");
        }
    }
}

void IO::JsonParser::ParseStatRequests(Json::Document& doc)
{
    auto root_map = doc.GetRoot().AsMap();
    const auto& stat_requests = root_map.at("stat_requests").AsArray();
    for (const auto& r : stat_requests) {
        const auto node = r.AsMap();
        std::string type = node.at("type").AsString();
        size_t request_id = node.at("id").AsInt();
        if (type == "Stop") {
            std::string name = node.at("name").AsString();
            _read.push_back(std::move(std::make_shared<Operations::StopStat>(name, request_id)));
        }
        else if (type == "Bus") {
            std::string name = node.at("name").AsString();
            _read.push_back(std::move(std::make_shared<Operations::BusStat>(name, request_id)));
        }
        else if (type == "Route") {
            std::string from = node.at("from").AsString();
            std::string to = node.at("to").AsString();
            _read.push_back(std::move(std::make_shared<Operations::FindWay>(from, to, request_id)));
        }
        else if (type == "Map") {
            _draw_map.push_back(std::move(std::make_shared<Operations::DrawMapOperation>(request_id)));
        }
        else {
            throw std::runtime_error("unknown read operation type");
        }
    }
}

void IO::JsonParser::ParseRenderSettings(Json::Document& doc)
{
    auto params = std::make_shared<Operations::MapBuilderSetParameters>();
    auto root_map = doc.GetRoot().AsMap();
    const auto& render_settings = root_map.at("render_settings").AsMap();
    params->
        SetWidth(render_settings.at("width").AsDouble())
        .SetHeight(render_settings.at("height").AsDouble())
        .SetPadding(render_settings.at("padding").AsDouble())
        .SetStopRadius(render_settings.at("stop_radius").AsDouble())
        .SetLineWidth(render_settings.at("line_width").AsDouble())
        .SetStopLabelSize(render_settings.at("stop_label_font_size").AsInt())
        .SetStopLabelOffset(render_settings.at("stop_label_offset").AsArray()[0].AsDouble(), render_settings.at("stop_label_offset").AsArray()[1].AsDouble())
        .SetBusLabelOffset(render_settings.at("bus_label_offset").AsArray()[0].AsDouble(), render_settings.at("bus_label_offset").AsArray()[1].AsDouble())
        .SetBusLabelSize(render_settings.at("bus_label_font_size").AsInt())
        .SetUnderlayerWidth(render_settings.at("underlayer_width").AsDouble());

    const auto& underlayer_color = render_settings.at("underlayer_color");
    if (underlayer_color.IsString()) {
        params->SetUnderlayerColor(underlayer_color.AsString());
    }
    else if (underlayer_color.IsArray())
    {
        auto arr = underlayer_color.AsArray();
        if (arr.size() == 3) {
            params->SetUnderlayerColor(arr[0].AsInt(), arr[1].AsInt(), arr[2].AsInt());
        }
        else if (arr.size() == 4) {
            params->SetUnderlayerColor(arr[0].AsInt(), arr[1].AsInt(), arr[2].AsInt(), arr[3].AsDouble());
        }
    }

    for (const auto& color_node : render_settings.at("color_palette").AsArray()) {
        if (color_node.IsString()) {
            params->AddColorToPalette(color_node.AsString());
        }
        else if (color_node.IsArray())
        {
            auto arr = color_node.AsArray();
            if (arr.size() == 3) {
                params->AddColorToPalette(arr[0].AsInt(), arr[1].AsInt(), arr[2].AsInt());
            }
            else if (arr.size() == 4) {
                params->AddColorToPalette(arr[0].AsInt(), arr[1].AsInt(), arr[2].AsInt(), arr[3].AsDouble());
            }
        }
    }

    for (const auto& l : render_settings.at("layers").AsArray()) {
        params->AddLayer(l.AsString());
    }

    _builder_setup.push_back(params);
}

std::shared_ptr<Operations::AddStop> IO::JsonParser::ParseAddStop(const std::map<std::string, Json::Node>& stop_node)
{
    Models::StopInfo info;
    info.name = stop_node.at("name").AsString();
    info.position.lat = Utils::ToRadians(stop_node.at("latitude").AsDouble());
    info.position.lon = Utils::ToRadians(stop_node.at("longitude").AsDouble());
    const auto distances = stop_node.at("road_distances").AsMap();
    for (const auto& p : distances) {
        info.known_distance[p.first] = p.second.AsDouble();
    }
    return std::make_shared<Operations::AddStop>(info);
}

std::shared_ptr<Operations::AddBus> IO::JsonParser::ParseAddBus(const std::map<std::string, Json::Node>& bus_node)
{
    Models::BusInfo info;
    info.name = bus_node.at("name").AsString();
    info.is_roundtrip = bus_node.at("is_roundtrip").AsBool();
    std::vector<Json::Node> stops = bus_node.at("stops").AsArray();
    for (const auto& s : stops) {
        info.stops.push_back(s.AsString());
    }
    if (!info.is_roundtrip && info.stops.size() > 1) {
        size_t read_index = info.stops.size() - 2;
        while (true)
        {
            info.stops.push_back(info.stops.at(read_index));
            if (read_index == 0) {
                break;
            }
            --read_index;
        }
    }

    return std::make_shared<Operations::AddBus>(info);
}
