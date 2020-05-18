#pragma once

#include "descriptions.h"
#include "json.h"
#include "svg.h"
#include "transport_router.h"
#include "transport_catalog.pb.h"

#include <map>
#include <string>
#include <unordered_map>
#include <vector>


struct RenderSettings {
  double max_width;
  double max_height;
  double padding;
  double outer_margin;
  std::vector<Svg::Color> palette;
  double line_width;
  Svg::Color underlayer_color;
  double underlayer_width;
  double stop_radius;
  Svg::Point bus_label_offset;
  int bus_label_font_size;
  Svg::Point stop_label_offset;
  int stop_label_font_size;
  std::vector<std::string> layers;
  double company_radius;
  double company_line_width;
};

class MapRenderer {
public:
  MapRenderer(const Descriptions::StopsDict& stops_dict,
              const Descriptions::BusesDict& buses_dict,
              const Descriptions::CompaniesDict& companies_dict,
              const Json::Dict& render_settings_json);

  MapRenderer(const serialization::Renderer& renderer);

  Svg::Document Render() const;
  Svg::Document RenderRoute(Svg::Document whole_map, const TransportRouter::RouteInfo& route) const;
  void Serialize(const Svg::Color& color, serialization::Color* color_data) const;
  void Seriliaze(serialization::Renderer& renderer) const;
  Svg::Color Deseriliaze(const serialization::Color& renderer) const;

private:
  RenderSettings render_settings_;
  std::map<std::string, Svg::Point> stops_coords_;
  std::unordered_map<std::string, Svg::Color> bus_colors_;
  // TODO: move instead of copy
  std::map<std::string, Descriptions::Bus> buses_dict_;
  std::unordered_map<int, Svg::Point> _comp_coords;

  void RenderBusLabel(Svg::Document& svg, const std::string& bus_name, const std::string& stop_name) const;
  void RenderStopPoint(Svg::Document& svg, Svg::Point point) const;
  void RenderStopLabel(Svg::Document& svg, Svg::Point point, const std::string& name) const;

  void RenderBusLines(Svg::Document& svg) const;
  void RenderBusLabels(Svg::Document& svg) const;
  void RenderStopPoints(Svg::Document& svg) const;
  void RenderStopLabels(Svg::Document& svg) const;

  void RenderRouteBusLines(Svg::Document& svg, const TransportRouter::RouteInfo& route) const;
  void RenderRouteBusLabels(Svg::Document& svg, const TransportRouter::RouteInfo& route) const;
  void RenderRouteStopPoints(Svg::Document& svg, const TransportRouter::RouteInfo& route) const;
  void RenderRouteStopLabels(Svg::Document& svg, const TransportRouter::RouteInfo& route) const;

  void RenderRouteCompanyLines(Svg::Document& svg, const TransportRouter::RouteInfo& route) const;
  void RenderRouteCompanyPoints(Svg::Document& svg, const TransportRouter::RouteInfo& route) const;
  void RenderRouteCompanyLabels(Svg::Document& svg, const TransportRouter::RouteInfo& route) const;

  void RenderStub(Svg::Document& svg) const;

  void ComputeCoordsByGrid(const Descriptions::StopsDict& stops_dict,
      const Descriptions::BusesDict& buses_dict,
      const Descriptions::CompaniesDict& companies_dict,
      const RenderSettings& render_settings);

  static const std::unordered_map<
      std::string,
      void (MapRenderer::*)(Svg::Document&) const
  > MAP_LAYER_ACTIONS;
  
  static const std::unordered_map<
      std::string,
      void (MapRenderer::*)(Svg::Document&, const TransportRouter::RouteInfo&) const
  > ROUTE_LAYER_ACTIONS;
};
