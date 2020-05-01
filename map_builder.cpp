#include "map_builder.h"

void Catalog::MapBuilder::Setup(const Models::MapBuilderParameters& params)
{
	_parameters = params;
}

std::unique_ptr<Svg::Document> Catalog::MapBuilder::DrawSvg(const Catalog::BusShedule& bus_schedule) const
{
	std::unique_ptr<Svg::Document> doc = std::make_unique<Svg::Document>();
	std::unique_ptr<Catalog::ComputationContext> context = GetContext(bus_schedule);
	for (const auto& l : _parameters.layers) {
		if (l == "bus_lines") {
			DrawBusLines(bus_schedule, *doc, *context);
		}
		else if (l == "bus_labels") {
			DrawBusLabels(bus_schedule, *doc, *context);
		}
		else if (l == "stop_points") {
			DrawStops(bus_schedule, *doc, *context);
		}
		else if (l == "stop_labels") {
			DrawStopLabels(bus_schedule, *doc, *context);
		}
	}
	return std::move(doc);
}

std::unique_ptr<Catalog::ComputationContext> Catalog::MapBuilder::GetContext(const Catalog::BusShedule& bus_schedule) const
{
	std::vector<std::shared_ptr<IStopProjection>> projections;
	projections.push_back(std::make_shared<UniformDistributionOperation>());
	projections.push_back(std::make_shared<CompressCoordinatesProjection>(_parameters));

	auto projection = std::make_shared<MultiStopProjection>(projections);

	std::unordered_map<std::string, Models::Stop> stops;
	for (const auto& stop : bus_schedule.GetStops()) {
		stops[stop.first] = stop.second;
	}

	projection->Project(bus_schedule, stops);

	return std::make_unique<Catalog::ComputationContext>(std::move(stops));
}

void Catalog::MapBuilder::DrawBusLines(const Catalog::BusShedule& bus_schedule, Svg::Document& document, const Catalog::ComputationContext& context) const
{
	size_t bus_index = 0;
	for (const auto& pair : bus_schedule.GetBuses()) {
		Svg::Color stroke = _parameters.color_palette[bus_index % _parameters.color_palette.size()];
		Svg::Polyline line;
		line.SetStrokeColor(stroke)
			.SetStrokeWidth(_parameters.line_width)
			.SetStrokeLineCap("round")
			.SetStrokeLineJoin("round");
		for (const std::string& stop : pair.second.stops) {
			line.AddPoint(context.GetStopPosition(stop));
		}
		document.Add(std::move(line));
		++bus_index;
	}
}

void Catalog::MapBuilder::DrawStops(const Catalog::BusShedule& bus_schedule, Svg::Document& document, const Catalog::ComputationContext& context) const
{
	for (const auto& s : bus_schedule.GetStops()) {
		Svg::Circle c;
		c.SetCenter(context.GetStopPosition(s.second.name))
			.SetRadius(_parameters.stop_radius)
			.SetFillColor("white");
		document.Add(std::move(c));
	}
}

void Catalog::MapBuilder::DrawStopLabels(const Catalog::BusShedule& bus_schedule, Svg::Document& document, const Catalog::ComputationContext& context) const
{
	for (const auto& s : bus_schedule.GetStops()) {
		Svg::Text label;
		Svg::Text underlayer;
		label.SetPoint(context.GetStopPosition(s.second.name))
			.SetOffset(_parameters.stop_label_offset)
			.SetFontSize(_parameters.stop_label_font_size)
			.SetFontFamily("Verdana")
			.SetData(s.first)
			.SetFillColor("black");
		underlayer.SetPoint(context.GetStopPosition(s.second.name))
			.SetOffset(_parameters.stop_label_offset)
			.SetFontSize(_parameters.stop_label_font_size)
			.SetFontFamily("Verdana")
			.SetData(s.first)
			.SetFillColor(_parameters.underlayer_color)
			.SetStrokeColor(_parameters.underlayer_color)
			.SetStrokeWidth(_parameters.underlayer_width)
			.SetStrokeLineCap("round")
			.SetStrokeLineJoin("round");
		document.Add(std::move(underlayer));
		document.Add(std::move(label));
	}
}

void Catalog::MapBuilder::DrawBusLabels(const Catalog::BusShedule& bus_schedule, Svg::Document& document, const Catalog::ComputationContext& context) const
{
	size_t bus_index = 0;
	for (const auto& p : bus_schedule.GetBuses()) {
		Svg::Color color = _parameters.color_palette[bus_index % _parameters.color_palette.size()];
		if (p.second.is_roundtrip) {
			const std::string& stop_name = p.second.stops[0];
			DrawBusLabel(document, context.GetStopPosition(stop_name), color, p.first);
		}
		else {
			const std::string& start_stop_name = p.second.stops[0];
			const std::string& finish_stop_name = p.second.stops[(p.second.stops.size() - 1) / 2];
			DrawBusLabel(document, context.GetStopPosition(start_stop_name), color, p.first);
			if (start_stop_name != finish_stop_name) {
				DrawBusLabel(document, context.GetStopPosition(finish_stop_name), color, p.first);
			}
			else {
				std::cerr << "same" << std::endl;
			}
		}
		bus_index++;
	}
}

void Catalog::MapBuilder::DrawBusLabel(Svg::Document& document, Svg::Point position, const Svg::Color& color, const std::string& name) const
{
	Svg::Text underlayer;
	underlayer
		.SetPoint(position)
		.SetOffset(_parameters.bus_label_offset)
		.SetFontSize(_parameters.bus_label_font_size)
		.SetFontFamily("Verdana")
		.SetFontWeight("bold")
		.SetData(name)
		.SetFillColor(_parameters.underlayer_color)
		.SetStrokeColor(_parameters.underlayer_color)
		.SetStrokeWidth(_parameters.underlayer_width)
		.SetStrokeLineCap("round")
		.SetStrokeLineJoin("round");
	Svg::Text label;
	label
		.SetPoint(position)
		.SetOffset(_parameters.bus_label_offset)
		.SetFontSize(_parameters.bus_label_font_size)
		.SetFontFamily("Verdana")
		.SetFontWeight("bold")
		.SetData(name)
		.SetFillColor(color);
	document.Add(underlayer);
	document.Add(label);
}
