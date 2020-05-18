#include "map_renderer.h"
#include "sphere.h"
#include <algorithm>
#include <iterator>

using namespace std;


static Svg::Point ParsePoint(const Json::Node& json) {
  const auto& array = json.AsArray();
  return {
      array[0].AsDouble(),
      array[1].AsDouble()
  };
}

static Svg::Color ParseColor(const Json::Node& json) {
  if (json.IsString()) {
    return json.AsString();
  }
  const auto& array = json.AsArray();
  assert(array.size() == 3 || array.size() == 4);
  Svg::Rgb rgb{
      static_cast<uint8_t>(array[0].AsInt()),
      static_cast<uint8_t>(array[1].AsInt()),
      static_cast<uint8_t>(array[2].AsInt())
  };
  if (array.size() == 3) {
    return rgb;
  } else {
    return Svg::Rgba{rgb, array[3].AsDouble()};
  }
}

static vector<Svg::Color> ParseColors(const Json::Node& json) {
  const auto& array = json.AsArray();
  vector<Svg::Color> colors;
  colors.reserve(array.size());
  transform(begin(array), end(array), back_inserter(colors), ParseColor);
  return colors;
}

RenderSettings ParseRenderSettings(const Json::Dict& json) {
  RenderSettings result;
  result.max_width = json.at("width").AsDouble();
  result.max_height = json.at("height").AsDouble();
  result.padding = json.at("padding").AsDouble();
  result.outer_margin = json.at("outer_margin").AsDouble();
  result.palette = ParseColors(json.at("color_palette"));
  result.line_width = json.at("line_width").AsDouble();
  result.underlayer_color = ParseColor(json.at("underlayer_color"));
  result.underlayer_width = json.at("underlayer_width").AsDouble();
  result.stop_radius = json.at("stop_radius").AsDouble();
  result.bus_label_offset = ParsePoint(json.at("bus_label_offset"));
  result.bus_label_font_size = json.at("bus_label_font_size").AsInt();
  result.stop_label_offset = ParsePoint(json.at("stop_label_offset"));
  result.stop_label_font_size = json.at("stop_label_font_size").AsInt();
  result.company_radius = json.at("company_radius").AsDouble();
  result.company_line_width = json.at("company_line_width").AsDouble();

  const auto& layers_array = json.at("layers").AsArray();
  result.layers.reserve(layers_array.size());
  for (const auto& layer_node : layers_array) {
    result.layers.push_back(layer_node.AsString());
  }

  return result;
}

static map<string, Descriptions::Bus> CopyBusesDict(const Descriptions::BusesDict& source) {
  map<string, Descriptions::Bus> target;
  for (const auto& [name, data_ptr] : source) {
    target.emplace(name, *data_ptr);
  }
  return target;
}

static unordered_set<string> FindSupportStops(const Descriptions::BusesDict& buses_dict) {
  unordered_set<string> support_stops;
  unordered_map<string, const Descriptions::Bus*> stops_first_bus;
  unordered_map<string, int> stops_rank;
  for (const auto& [_, bus_ptr] : buses_dict) {
    for (const string& stop : bus_ptr->endpoints) {
      support_stops.insert(stop);
    }
    for (const string& stop : bus_ptr->stops) {
      ++stops_rank[stop];
      const auto [it, inserted] = stops_first_bus.emplace(stop, bus_ptr);
      if (!inserted && it->second != bus_ptr) {
        support_stops.insert(stop);
      }
    }
  }

  for (const auto& [stop, rank] : stops_rank) {
    if (rank > 2) {
      support_stops.insert(stop);
    }
  }

  return support_stops;
}

static unordered_map<string, Sphere::Point> ComputeInterpolatedStopsGeoCoords(
    const Descriptions::StopsDict& stops_dict,
    const Descriptions::BusesDict& buses_dict
) {
  const unordered_set<string> support_stops = FindSupportStops(buses_dict);

  unordered_map<string, Sphere::Point> stops_coords;
  for (const auto& [stop_name, stop_ptr] : stops_dict) {
    stops_coords[stop_name] = stop_ptr->position;
  }

  for (const auto& [_, bus_ptr] : buses_dict) {
    const auto& stops = bus_ptr->stops;
    if (stops.empty()) {
      continue;
    }
    size_t last_support_idx = 0;
    stops_coords[stops[0]] = stops_dict.at(stops[0])->position;
    for (size_t stop_idx = 1; stop_idx < stops.size(); ++stop_idx) {
      if (support_stops.count(stops[stop_idx])) {
        const Sphere::Point prev_coord = stops_dict.at(stops[last_support_idx])->position;
        const Sphere::Point next_coord = stops_dict.at(stops[stop_idx])->position;
        const double lat_step = (next_coord.latitude - prev_coord.latitude) / (stop_idx - last_support_idx);
        const double lon_step = (next_coord.longitude - prev_coord.longitude) / (stop_idx - last_support_idx);
        for (size_t middle_stop_idx = last_support_idx + 1; middle_stop_idx < stop_idx; ++middle_stop_idx) {
          stops_coords[stops[middle_stop_idx]] = {
              prev_coord.latitude + lat_step * (middle_stop_idx - last_support_idx),
              prev_coord.longitude + lon_step * (middle_stop_idx - last_support_idx),
          };
        }
        stops_coords[stops[stop_idx]] = stops_dict.at(stops[stop_idx])->position;
        last_support_idx = stop_idx;
      }
    }
  }

  return stops_coords;
}

struct NeighboursDicts {
  unordered_map<double, vector<double>> neighbour_lats;
  unordered_map<double, vector<double>> neighbour_lons;
};

static NeighboursDicts BuildCoordNeighboursDicts(const unordered_map<string, Sphere::Point>& stops_coords,
    const Descriptions::BusesDict& buses_dict,
    const Descriptions::CompaniesDict& comp_dict) {
    unordered_map<double, vector<double>> neighbour_lats;
    unordered_map<double, vector<double>> neighbour_lons;
    for (const auto& [bus_name, bus_ptr] : buses_dict) {
        const auto& stops = bus_ptr->stops;
        if (stops.empty()) {
            continue;
        }
        Sphere::Point point_prev = stops_coords.at(stops[0]);
        for (size_t stop_idx = 1; stop_idx < stops.size(); ++stop_idx) {
            const auto point_cur = stops_coords.at(stops[stop_idx]);
            if (stops[stop_idx] != stops[stop_idx - 1]) {
                const auto [min_lat, max_lat] = minmax(point_prev.latitude, point_cur.latitude);
                const auto [min_lon, max_lon] = minmax(point_prev.longitude, point_cur.longitude);
                neighbour_lats[max_lat].push_back(min_lat);
                neighbour_lons[max_lon].push_back(min_lon);
            }
            point_prev = point_cur;
        }
    }

    for (const auto& company : comp_dict) {
        for (const auto& stop : company.second.nearby_stops) {
            const auto stop_cur = stops_coords.at(stop.first);
            const auto [min_lat, max_lat] = minmax(stop_cur.latitude, company.second.lat);
            const auto [min_lon, max_lon] = minmax(stop_cur.longitude, company.second.lon);
            neighbour_lats[max_lat].push_back(min_lat);
            neighbour_lons[max_lon].push_back(min_lon);
        }
    }

    for (auto* neighbours_dict : { &neighbour_lats, &neighbour_lons }) {
        for (auto& [_, values] : *neighbours_dict) {
            sort(begin(values), end(values));
            values.erase(unique(begin(values), end(values)), end(values));
        }
    }

    return { move(neighbour_lats), move(neighbour_lons) };
}

class CoordsCompressor {
public:
  CoordsCompressor(const unordered_map<string, Sphere::Point>& stops_coords, const Descriptions::CompaniesDict& companies_dict) {
    for (const auto& [_, coord] : stops_coords) {
      lats_.push_back({coord.latitude});
      lons_.push_back({coord.longitude});
    }

    for (const auto& company : companies_dict) {
        lats_.push_back({ company.second.lat });
        lons_.push_back({ company.second.lon });
    }

    sort(begin(lats_), end(lats_));
    sort(begin(lons_), end(lons_));
  }

  void FillIndices(const unordered_map<double, vector<double>>& neighbour_lats,
                   const unordered_map<double, vector<double>>& neighbour_lons) {
    FillCoordIndices(lats_, neighbour_lats);
    FillCoordIndices(lons_, neighbour_lons);
  }

  void FillTargets(double max_width, double max_height, double padding) {
    if (lats_.empty() || lons_.empty()) {
      return;
    }

    const size_t max_lat_idx = FindMaxLatIdx();
    const double y_step = max_lat_idx ? (max_height - 2 * padding) / max_lat_idx : 0;

    const size_t max_lon_idx = FindMaxLonIdx();
    const double x_step = max_lon_idx ? (max_width - 2 * padding) / max_lon_idx : 0;

    for (auto& [_, idx, value] : lats_) {
      value = max_height - padding - idx * y_step;
    }
    for (auto& [_, idx, value] : lons_) {
      value = idx * x_step + padding;
    }
  }

  double MapLat(double value) const {
    return Find(lats_, value).target;
  }
  double MapLon(double value) const {
    return Find(lons_, value).target;
  }

private:
  struct CoordInfo {
    double source;
    size_t idx = 0;
    double target = 0;

    bool operator<(const CoordInfo& other) const {
      return source < other.source;
    }
  };

  vector<CoordInfo> lats_;
  vector<CoordInfo> lons_;

  void FillCoordIndices(vector<CoordInfo>& coords, const unordered_map<double, vector<double>>& neighbour_values) {
    for (auto coord_it = begin(coords); coord_it != end(coords); ++coord_it) {
      const auto neighbours_it = neighbour_values.find(coord_it->source);
      if (neighbours_it == neighbour_values.end()) {
        coord_it->idx = 0;
        continue;
      }
      const auto& neighbours = neighbours_it->second;
      optional<size_t> max_neighbour_idx;
      for (const double value : neighbours) {
        const size_t idx = Find(coords, value, coord_it).idx;
        if (idx > max_neighbour_idx) {
          max_neighbour_idx = idx;
        }
      }
      coord_it->idx = *max_neighbour_idx + 1;
    }
  }

  static const CoordInfo& Find(const vector<CoordInfo>& sorted_values,
                               double value,
                               optional<vector<CoordInfo>::const_iterator> end_it = nullopt) {
    return *lower_bound(begin(sorted_values), end_it.value_or(end(sorted_values)), CoordInfo{value});
  }

  static size_t FindMaxIdx(const vector<CoordInfo>& coords) {
    return max_element(
        begin(coords), end(coords),
        [](const CoordInfo& lhs, const CoordInfo& rhs) {
          return lhs.idx < rhs.idx;
        }
    )->idx;
  }

  size_t FindMaxLatIdx() const {
    return FindMaxIdx(lats_);
  }
  size_t FindMaxLonIdx() const {
    return FindMaxIdx(lons_);
  }
};

static unordered_map<string, Svg::Color> ChooseBusColors(const Descriptions::BusesDict& buses_dict,
                                                         const RenderSettings& render_settings) {
  const auto& palette = render_settings.palette;
  unordered_map<string, Svg::Color> bus_colors;
  int idx = 0;
  for (const auto& [bus_name, bus_ptr] : buses_dict) {
    bus_colors[bus_name] = palette[idx++ % palette.size()];
  }
  return bus_colors;
}

MapRenderer::MapRenderer(const Descriptions::StopsDict& stops_dict,
                         const Descriptions::BusesDict& buses_dict,
                         const Descriptions::CompaniesDict& companies_dict,
                         const Json::Dict& render_settings_json)
    : render_settings_(ParseRenderSettings(render_settings_json)),
      bus_colors_(ChooseBusColors(buses_dict, render_settings_)),
      buses_dict_(CopyBusesDict(buses_dict))
{
    ComputeCoordsByGrid(stops_dict, buses_dict, companies_dict, render_settings_);
}

MapRenderer::MapRenderer(const serialization::Renderer& renderer)
{
    render_settings_.max_width = renderer.settings().max_width();
    render_settings_.max_height = renderer.settings().max_height();
    render_settings_.padding = renderer.settings().padding();
    render_settings_.outer_margin = renderer.settings().outer_margin();
    for (int i = 0; i < renderer.settings().palette_size(); ++i) {
        render_settings_.palette.push_back(Deseriliaze(renderer.settings().palette(i)));
    }
    render_settings_.line_width = renderer.settings().line_width();
    render_settings_.underlayer_color = Deseriliaze(renderer.settings().underlayer_color());
    render_settings_.underlayer_width = renderer.settings().underlayer_width();
    render_settings_.stop_radius = renderer.settings().stop_radius();
    render_settings_.bus_label_offset.x = renderer.settings().bus_label_offset().x();
    render_settings_.bus_label_offset.y = renderer.settings().bus_label_offset().y();
    render_settings_.bus_label_font_size = renderer.settings().bus_label_font_size();
    render_settings_.stop_label_offset.x = renderer.settings().stop_label_offset().x();
    render_settings_.stop_label_offset.y = renderer.settings().stop_label_offset().y();
    render_settings_.stop_label_font_size = renderer.settings().stop_label_font_size();
    render_settings_.company_radius = renderer.settings().company_radius();
    render_settings_.company_line_width = renderer.settings().company_line_width();
    for (int i = 0; i < renderer.settings().layers_size(); ++i) {
        render_settings_.layers.push_back(renderer.settings().layers(i));
    }

    for (const auto& p : renderer.stops_coords()) {
        stops_coords_[p.first].x = p.second.x();
        stops_coords_[p.first].y = p.second.y();
    }

    for (const auto& p : renderer.bus_colors()) {
        bus_colors_[p.first] = Deseriliaze(p.second);
    }

    for (const auto& p : renderer.buses()) {
        buses_dict_[p.first].name = p.first;
        for (const auto& s : p.second.stops()) {
            buses_dict_[p.first].stops.push_back(s);
        }
        for (const auto& e : p.second.endpoints()) {
            buses_dict_[p.first].endpoints.push_back(e);
        }
    }

    for (const auto& p : renderer.company_coords()) {
        _comp_coords[p.first].x = p.second.x();
        _comp_coords[p.first].y = p.second.y();
    }
}

using RouteBusItem = TransportRouter::RouteInfo::BusItem;
using RouteWaitItem = TransportRouter::RouteInfo::WaitItem;
using RouteWalkToCompanyItem = TransportRouter::RouteInfo::WalkToCompany;

void MapRenderer::RenderBusLines(Svg::Document& svg) const {
  for (const auto& [bus_name, bus] : buses_dict_) {
    const auto& stops = bus.stops;
    if (stops.empty()) {
      continue;
    }
    Svg::Polyline line;
    line.SetStrokeColor(bus_colors_.at(bus_name))
        .SetStrokeWidth(render_settings_.line_width)
        .SetStrokeLineCap("round").SetStrokeLineJoin("round");
    for (const auto& stop_name : stops) {
      line.AddPoint(stops_coords_.at(stop_name));
    }
    svg.Add(line);
  }
}

void MapRenderer::RenderRouteBusLines(Svg::Document& svg, const TransportRouter::RouteInfo& route) const {
    for (const auto& item : route.items) {
        if (!holds_alternative<RouteBusItem>(item)) {
            continue;
        }
        const auto& bus_item = get<RouteBusItem>(item);
        const string& bus_name = bus_item.bus_name;
        const auto& stops = buses_dict_.at(bus_name).stops;
        if (stops.empty()) {
            continue;
        }
        Svg::Polyline line;
        line.SetStrokeColor(bus_colors_.at(bus_name))
            .SetStrokeWidth(render_settings_.line_width)
            .SetStrokeLineCap("round").SetStrokeLineJoin("round");
        for (size_t stop_idx = bus_item.start_stop_idx; stop_idx <= bus_item.finish_stop_idx; ++stop_idx) {
            const string& stop_name = stops[stop_idx];
            line.AddPoint(stops_coords_.at(stop_name));
        }
        svg.Add(line);
    }
}

void MapRenderer::RenderBusLabel(Svg::Document& svg, const string& bus_name, const string& stop_name) const {
  const auto& color = bus_colors_.at(bus_name);  // can be optimized a bit by moving upper
  const auto point = stops_coords_.at(stop_name);
  const auto base_text =
      Svg::Text{}
      .SetPoint(point)
      .SetOffset(render_settings_.bus_label_offset)
      .SetFontSize(render_settings_.bus_label_font_size)
      .SetFontFamily("Verdana")
      .SetFontWeight("bold")
      .SetData(bus_name);
  svg.Add(
      Svg::Text(base_text)
      .SetFillColor(render_settings_.underlayer_color)
      .SetStrokeColor(render_settings_.underlayer_color)
      .SetStrokeWidth(render_settings_.underlayer_width)
      .SetStrokeLineCap("round").SetStrokeLineJoin("round")
  );
  svg.Add(
      Svg::Text(base_text)
      .SetFillColor(color)
  );
}

void MapRenderer::RenderBusLabels(Svg::Document& svg) const {
  for (const auto& [bus_name, bus] : buses_dict_) {
    const auto& stops = bus.stops;
    if (!stops.empty()) {
      for (const string& endpoint : bus.endpoints) {
        RenderBusLabel(svg, bus_name, endpoint);
      }
    }
  }
}

void MapRenderer::RenderRouteBusLabels(Svg::Document& svg, const TransportRouter::RouteInfo& route) const {
  for (const auto& item : route.items) {
    // TODO: remove copypaste with bus lines rendering
    if (!holds_alternative<RouteBusItem>(item)) {
      continue;
    }
    const auto& bus_item = get<RouteBusItem>(item);
    const string& bus_name = bus_item.bus_name;
    const auto& bus = buses_dict_.at(bus_name);
    const auto& stops = bus.stops;
    if (stops.empty()) {
      continue;
    }
    for (const size_t stop_idx : {bus_item.start_stop_idx, bus_item.finish_stop_idx}) {
      const auto stop_name = stops[stop_idx];
      if (stop_idx == 0 
          || stop_idx == stops.size() - 1 
          || find(begin(bus.endpoints), end(bus.endpoints), stop_name) != end(bus.endpoints)
      ) {
        RenderBusLabel(svg, bus_name, stop_name);
      }
    }
  }
}

void MapRenderer::RenderStopPoint(Svg::Document& svg, Svg::Point point) const {
  svg.Add(Svg::Circle{}
          .SetCenter(point)
          .SetRadius(render_settings_.stop_radius)
          .SetFillColor("white"));
}

void MapRenderer::RenderStopPoints(Svg::Document& svg) const {
  for (const auto& [_, stop_point] : stops_coords_) {
    RenderStopPoint(svg, stop_point);
  }
}

void MapRenderer::RenderRouteStopPoints(Svg::Document& svg, const TransportRouter::RouteInfo& route) const {
    for (const auto& item : route.items) {
        // TODO: remove copypaste with bus lines rendering
        if (!holds_alternative<RouteBusItem>(item)) {
            continue;
        }
        const auto& bus_item = get<RouteBusItem>(item);
        const string& bus_name = bus_item.bus_name;
        const auto& stops = buses_dict_.at(bus_name).stops;
        if (stops.empty()) {
            continue;
        }
        for (size_t stop_idx = bus_item.start_stop_idx; stop_idx <= bus_item.finish_stop_idx; ++stop_idx) {
            const string& stop_name = stops[stop_idx];
            RenderStopPoint(svg, stops_coords_.at(stop_name));
        }
    }
}

void MapRenderer::RenderStopLabel(Svg::Document& svg, Svg::Point point, const string& name) const {
  auto base_text =
      Svg::Text{}
      .SetPoint(point)
      .SetOffset(render_settings_.stop_label_offset)
      .SetFontSize(render_settings_.stop_label_font_size)
      .SetFontFamily("Verdana")
      .SetData(name);
  svg.Add(
      Svg::Text(base_text)
      .SetFillColor(render_settings_.underlayer_color)
      .SetStrokeColor(render_settings_.underlayer_color)
      .SetStrokeWidth(render_settings_.underlayer_width)
      .SetStrokeLineCap("round").SetStrokeLineJoin("round")
  );
  svg.Add(
      base_text
      .SetFillColor("black")
  );
}

void MapRenderer::RenderStopLabels(Svg::Document& svg) const {
  for (const auto& [stop_name, stop_point] : stops_coords_) {
    RenderStopLabel(svg, stop_point, stop_name);
  }
}

void MapRenderer::RenderRouteStopLabels(Svg::Document& svg, const TransportRouter::RouteInfo& route) const {
  if (route.items.empty()) {
    return;
  }
  for (const auto& item : route.items) {
    if (!holds_alternative<RouteWaitItem>(item)) {
      continue;
    }
    const auto& wait_item = get<RouteWaitItem>(item);
    const string& stop_name = wait_item.stop_name;
    RenderStopLabel(svg, stops_coords_.at(stop_name), stop_name);
  }

  // draw stop label for last stop
  if (holds_alternative<RouteBusItem>(route.items.back())) {
      const auto& last_bus_item = get<RouteBusItem>(route.items.back());
      const string& last_stop_name = buses_dict_.at(last_bus_item.bus_name).stops[last_bus_item.finish_stop_idx];
      RenderStopLabel(svg, stops_coords_.at(last_stop_name), last_stop_name);
  }

  if (holds_alternative<RouteWalkToCompanyItem>(route.items.back())) {
      const auto& walk_item = get<RouteWalkToCompanyItem>(route.items.back());
      RenderStopLabel(svg, stops_coords_.at(walk_item.stop_name), walk_item.stop_name);
  }
}

void MapRenderer::RenderRouteCompanyLines(Svg::Document& svg, const TransportRouter::RouteInfo& route) const
{
    for (const auto& item : route.items) {
        if (!holds_alternative<RouteWalkToCompanyItem>(item)) {
            continue;
        }
        const auto& walk_item = get<RouteWalkToCompanyItem>(item);
        const string& stop_name = walk_item.stop_name;
        Svg::Polyline line;
        line.SetStrokeColor("black")
            .SetStrokeWidth(render_settings_.company_line_width)
            .SetStrokeLineCap("round").SetStrokeLineJoin("round");
        line.AddPoint(stops_coords_.at(stop_name));
        line.AddPoint(_comp_coords.at(walk_item.company_id));
        svg.Add(line);
    }
}

void MapRenderer::RenderRouteCompanyPoints(Svg::Document& svg, const TransportRouter::RouteInfo& route) const
{
    for (const auto& item : route.items) {
        // TODO: remove copypaste with bus lines rendering
        if (!holds_alternative<RouteWalkToCompanyItem>(item)) {
            continue;
        }
        const auto& walk_item = get<RouteWalkToCompanyItem>(item);
        svg.Add(Svg::Circle{}
            .SetCenter(_comp_coords.at(walk_item.company_id))
            .SetRadius(render_settings_.company_radius)
            .SetFillColor("black"));
    }
}

void MapRenderer::RenderRouteCompanyLabels(Svg::Document& svg, const TransportRouter::RouteInfo& route) const
{
    if (route.items.empty()) {
        return;
    }
    for (const auto& item : route.items) {
        if (!holds_alternative<RouteWalkToCompanyItem>(item)) {
            continue;
        }
        const auto& walk_item = get<RouteWalkToCompanyItem>(item);
        const string& company_name = walk_item.company_display_name;
        RenderStopLabel(svg, _comp_coords.at(walk_item.company_id), company_name);
    }
}

void MapRenderer::RenderStub(Svg::Document& svg) const
{
}

void MapRenderer::ComputeCoordsByGrid(const Descriptions::StopsDict& stops_dict, const Descriptions::BusesDict& buses_dict, const Descriptions::CompaniesDict& companies_dict, const RenderSettings& render_settings)
{
    const auto stops_coords = ComputeInterpolatedStopsGeoCoords(stops_dict, buses_dict);
    const auto [neighbour_lats, neighbour_lons] = BuildCoordNeighboursDicts(stops_coords, buses_dict, companies_dict);

    CoordsCompressor compressor(stops_coords, companies_dict);
    compressor.FillIndices(neighbour_lats, neighbour_lons);
    compressor.FillTargets(render_settings.max_width, render_settings.max_height, render_settings.padding);

    for (const auto& [stop_name, coord] : stops_coords) {
        stops_coords_[stop_name] = { compressor.MapLon(coord.longitude), compressor.MapLat(coord.latitude) };
    }

    for (const auto& company : companies_dict) {
        _comp_coords[company.first] = { compressor.MapLon(company.second.lon), compressor.MapLat(company.second.lat) };
    }
}

const unordered_map<
    string,
    void (MapRenderer::*)(Svg::Document&) const
> MapRenderer::MAP_LAYER_ACTIONS = {
    {"bus_lines",   &MapRenderer::RenderBusLines},
    {"bus_labels",  &MapRenderer::RenderBusLabels},
    {"stop_points", &MapRenderer::RenderStopPoints},
    {"stop_labels", &MapRenderer::RenderStopLabels},
    {"company_lines", &MapRenderer::RenderStub},
    {"company_points", &MapRenderer::RenderStub},
    {"company_labels", &MapRenderer::RenderStub},
};

const unordered_map<
    string,
    void (MapRenderer::*)(Svg::Document&, const TransportRouter::RouteInfo&) const
> MapRenderer::ROUTE_LAYER_ACTIONS = {
    {"bus_lines",   &MapRenderer::RenderRouteBusLines},
    {"bus_labels",  &MapRenderer::RenderRouteBusLabels},
    {"stop_points", &MapRenderer::RenderRouteStopPoints},
    {"stop_labels", &MapRenderer::RenderRouteStopLabels},
    {"company_lines", &MapRenderer::RenderRouteCompanyLines},
    {"company_points", &MapRenderer::RenderRouteCompanyPoints},
    {"company_labels", &MapRenderer::RenderRouteCompanyLabels},
};

Svg::Document MapRenderer::Render() const {
  Svg::Document svg;

  for (const auto& layer : render_settings_.layers) {
    (this->*MAP_LAYER_ACTIONS.at(layer))(svg);
  }

  return svg;
}

Svg::Document MapRenderer::RenderRoute(
    Svg::Document svg,
    const TransportRouter::RouteInfo& route
) const {
  const double outer_margin = render_settings_.outer_margin;
  svg.Add(
      Svg::Rectangle{}
      .SetFillColor(render_settings_.underlayer_color)
      .SetTopLeftPoint({-outer_margin, -outer_margin})
      .SetBottomRightPoint({
          render_settings_.max_width + outer_margin,
          render_settings_.max_height + outer_margin
      })
  );

  for (const auto& layer : render_settings_.layers) {
    (this->*ROUTE_LAYER_ACTIONS.at(layer))(svg, route);
  }

  return svg;
}

void MapRenderer::Serialize(const Svg::Color& color, serialization::Color* color_data) const
{
    if (color.index() == 0) {
        color_data->set_type(serialization::ColorType::NOT_DEFINED);
    }
    else if (color.index() == 1) {
        color_data->set_type(serialization::ColorType::STRING);
        color_data->set_color_string(std::get<std::string>(color));
    }
    else if (color.index() == 2) {
        color_data->set_type(serialization::ColorType::RGB);
        color_data->set_red(std::get<Svg::Rgb>(color).red);
        color_data->set_green(std::get<Svg::Rgb>(color).green);
        color_data->set_blue(std::get<Svg::Rgb>(color).blue);
    }
    else if (color.index() == 3) {
        color_data->set_type(serialization::ColorType::RGBA);
        color_data->set_red(std::get<Svg::Rgba>(color).red);
        color_data->set_green(std::get<Svg::Rgba>(color).green);
        color_data->set_blue(std::get<Svg::Rgba>(color).blue);
        color_data->set_opacity(std::get<Svg::Rgba>(color).opacity);
    }
}

void MapRenderer::Seriliaze(serialization::Renderer& renderer) const
{
    auto* settings = renderer.mutable_settings();
    settings->set_max_width(render_settings_.max_width);
    settings->set_max_height(render_settings_.max_height);
    settings->set_padding(render_settings_.padding);
    settings->set_outer_margin(render_settings_.outer_margin);
    for (const auto& color : render_settings_.palette) {
        auto* color_data = settings->add_palette();
        Serialize(color, color_data);
    }
    settings->set_line_width(render_settings_.line_width);
    Serialize(render_settings_.underlayer_color, settings->mutable_underlayer_color());
    settings->set_underlayer_width(render_settings_.underlayer_width);
    settings->set_stop_radius(render_settings_.stop_radius);
    auto* bus_label_offset = settings->mutable_bus_label_offset();
    bus_label_offset->set_x(render_settings_.bus_label_offset.x);
    bus_label_offset->set_y(render_settings_.bus_label_offset.y);
    settings->set_bus_label_font_size(render_settings_.bus_label_font_size);
    auto* stop_label_offset = settings->mutable_stop_label_offset();
    stop_label_offset->set_x(render_settings_.stop_label_offset.x);
    stop_label_offset->set_y(render_settings_.stop_label_offset.y);
    settings->set_stop_label_font_size(render_settings_.stop_label_font_size);
    settings->set_company_radius(render_settings_.company_radius);
    settings->set_company_line_width(render_settings_.company_line_width);
    for (const auto& layer : render_settings_.layers) {
        settings->add_layers(layer);
    }

    for (const auto& p : stops_coords_) {
        (*renderer.mutable_stops_coords())[p.first].set_x(p.second.x);
        (*renderer.mutable_stops_coords())[p.first].set_y(p.second.y);
    }

    for (const auto& p : bus_colors_) {
        Serialize(p.second, &(*renderer.mutable_bus_colors())[p.first]);
    }

    for (const auto& p : buses_dict_) {
        for (const auto& stop : p.second.stops) {
            (*renderer.mutable_buses())[p.first].add_stops(stop);
        }
        for (const auto& endpoint : p.second.endpoints) {
            (*renderer.mutable_buses())[p.first].add_endpoints(endpoint);
        }
    }

    for (const auto& p : _comp_coords) {
        (*renderer.mutable_company_coords())[p.first].set_x(p.second.x);
        (*renderer.mutable_company_coords())[p.first].set_y(p.second.y);
    }
}

Svg::Color MapRenderer::Deseriliaze(const serialization::Color& renderer) const
{
    if (renderer.type() == serialization::ColorType::NOT_DEFINED) {
        return Svg::Color(std::monostate());
    }
    else if (renderer.type() == serialization::ColorType::STRING) {
        return Svg::Color(renderer.color_string());
    }
    else if (renderer.type() == serialization::ColorType::RGB) {
        Svg::Rgb rgb;
        rgb.red = renderer.red();
        rgb.green = renderer.green();
        rgb.blue = renderer.blue();
        return Svg::Color(rgb);
    }
    else {
        Svg::Rgba rgb;
        rgb.red = renderer.red();
        rgb.green = renderer.green();
        rgb.blue = renderer.blue();
        rgb.opacity = renderer.opacity();
        return Svg::Color(rgb);
    }
}
