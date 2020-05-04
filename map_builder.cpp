#include "map_builder.h"

void Catalog::MapBuilder::Setup(const Models::MapBuilderParameters& params)
{
	_parameters = params;
}

std::unique_ptr<Svg::Document> Catalog::MapBuilder::DrawSvg(const Catalog::BusShedule& bus_schedule) const
{
	std::unique_ptr<Svg::Document> doc = std::make_unique<Svg::Document>();
	GetContext(bus_schedule);
	for (const auto& l : _parameters.layers) {
		if (l == "bus_lines") {
			DrawBusLines(bus_schedule, *doc, *_context);
		}
		else if (l == "bus_labels") {
			DrawBusLabels(bus_schedule, *doc, *_context);
		}
		else if (l == "stop_points") {
			DrawStops(bus_schedule, *doc, *_context);
		}
		else if (l == "stop_labels") {
			DrawStopLabels(bus_schedule, *doc, *_context);
		}
	}
	return std::move(doc);
}

std::unique_ptr<Svg::Document> Catalog::MapBuilder::DrawRoute(const Catalog::BusShedule& bus_schedule, const Models::RouteInfo& route) const
{
	std::unique_ptr<Svg::Document> doc = DrawSvg(bus_schedule);
	GetContext(bus_schedule);
	Svg::Rectangle rect;
	rect.SetLeftUpper(Svg::Point(-_parameters.outer_margin, -_parameters.outer_margin))
		.SetWidht(_parameters.width + 2 * _parameters.outer_margin)
		.SetHeight(_parameters.height + 2 * _parameters.outer_margin)
		.SetFillColor(_parameters.underlayer_color);
	doc->Add(rect);
	for (const auto& l : _parameters.layers) {
		if (l == "bus_lines") {
			DrawBusLines(route, *doc, *_context);
		}
		else if (l == "bus_labels") {
			DrawBusLabels(bus_schedule, route, *doc, *_context);
		}
		else if (l == "stop_points") {
			DrawStops(route, *doc, *_context);
		}
		else if (l == "stop_labels") {
			DrawStopLabels(route, *doc, *_context);
		}
	}
	return std::move(doc);
}

void Catalog::MapBuilder::GetContext(const Catalog::BusShedule& bus_schedule) const
{
	if (_context != nullptr) {
		return;
	}

	std::vector<std::shared_ptr<IStopProjection>> projections;
	projections.push_back(std::make_shared<UniformDistributionOperation>());
	projections.push_back(std::make_shared<CompressCoordinatesProjection>(_parameters));

	auto projection = std::make_shared<MultiStopProjection>(projections);

	std::unordered_map<std::string, Models::Stop> stops;
	for (const auto& stop : bus_schedule.GetStops()) {
		stops[stop.first] = stop.second;
	}

	projection->Project(bus_schedule, stops);

	_context = std::move(std::make_unique<Catalog::ComputationContext>(std::move(stops)));
	size_t bus_index = 0;
	for (const auto& pair : bus_schedule.GetBuses()) {
		_context->SetBusIndex(pair.first, bus_index);
		++bus_index;
	}
}

void Catalog::MapBuilder::DrawBusLines(const Catalog::BusShedule& bus_schedule, Svg::Document& document, const Catalog::ComputationContext& context) const
{
	for (const auto& pair : bus_schedule.GetBuses()) {
		Svg::Color stroke = _parameters.color_palette[context.GetBusIndex(pair.first) % _parameters.color_palette.size()];
		Svg::Polyline line;
		line.SetStrokeColor(stroke)
			.SetStrokeWidth(_parameters.line_width)
			.SetStrokeLineCap("round")
			.SetStrokeLineJoin("round");
		for (const std::string& stop : pair.second.stops) {
			line.AddPoint(context.GetStopPosition(stop));
		}
		document.Add(std::move(line));
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
		DrawStopLabel(context, s.first, document);
	}
}

void Catalog::MapBuilder::DrawBusLabels(const Catalog::BusShedule& bus_schedule, Svg::Document& document, const Catalog::ComputationContext& context) const
{
	for (const auto& p : bus_schedule.GetBuses()) {
		Svg::Color color = _parameters.color_palette[context.GetBusIndex(p.first) % _parameters.color_palette.size()];
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

void Catalog::MapBuilder::DrawBusLines(const Models::RouteInfo& route, Svg::Document& document, const Catalog::ComputationContext& context) const
{
	for (const auto& segment : route.segments) {
		Svg::Color stroke = _parameters.color_palette[context.GetBusIndex(segment.bus) % _parameters.color_palette.size()];
		Svg::Polyline line;
		line.SetStrokeColor(stroke)
			.SetStrokeWidth(_parameters.line_width)
			.SetStrokeLineCap("round")
			.SetStrokeLineJoin("round");
		line.AddPoint(context.GetStopPosition(segment.stop_from));
		for (const std::string_view& stop : segment.stops) {
			line.AddPoint(context.GetStopPosition(std::string(stop)));
		}
		document.Add(std::move(line));
	}
}

void Catalog::MapBuilder::DrawStops(const Models::RouteInfo& route, Svg::Document& document, const Catalog::ComputationContext& context) const
{
	for (const auto& segment : route.segments) {
		Svg::Circle c;
		c.SetCenter(context.GetStopPosition(segment.stop_from))
			.SetRadius(_parameters.stop_radius)
			.SetFillColor("white");
		document.Add(std::move(c));

		for (const std::string_view& stop : segment.stops) {
			Svg::Circle c;
			c.SetCenter(context.GetStopPosition(std::string(stop)))
				.SetRadius(_parameters.stop_radius)
				.SetFillColor("white");
			document.Add(std::move(c));
		}
	}
}

void Catalog::MapBuilder::DrawStopLabels(const Models::RouteInfo& route, Svg::Document& document, const Catalog::ComputationContext& context) const
{
	std::string last = "";
	for (const auto& segment : route.segments) {
		DrawStopLabel(context, segment.stop_from, document);
		last = segment.stops[segment.stops.size() - 1];
	}
	if (route.segments.size() > 0 && route.segments[route.segments.size() - 1].stops.size() > 0) {
		const std::vector<std::string_view>& last_segment = route.segments[route.segments.size() - 1].stops;
		const std::string_view& last = last_segment.at(last_segment.size() - 1);
		DrawStopLabel(context, std::string(last), document);
	}
}

void Catalog::MapBuilder::DrawStopLabel(const Catalog::ComputationContext& context, const std::string& stop, Svg::Document& document) const
{
	Svg::Text label;
	Svg::Text underlayer;
	label.SetPoint(context.GetStopPosition(stop))
		.SetOffset(_parameters.stop_label_offset)
		.SetFontSize(_parameters.stop_label_font_size)
		.SetFontFamily("Verdana")
		.SetData(stop)
		.SetFillColor("black");
	underlayer.SetPoint(context.GetStopPosition(stop))
		.SetOffset(_parameters.stop_label_offset)
		.SetFontSize(_parameters.stop_label_font_size)
		.SetFontFamily("Verdana")
		.SetData(stop)
		.SetFillColor(_parameters.underlayer_color)
		.SetStrokeColor(_parameters.underlayer_color)
		.SetStrokeWidth(_parameters.underlayer_width)
		.SetStrokeLineCap("round")
		.SetStrokeLineJoin("round");
	document.Add(std::move(underlayer));
	document.Add(std::move(label));
}

void Catalog::MapBuilder::DrawBusLabels(const Catalog::BusShedule& bus_schedule, const Models::RouteInfo& route, Svg::Document& document, const Catalog::ComputationContext& context) const
{
	for (const auto& segment : route.segments) {
		const std::string& from = segment.stop_from;
		const std::string_view& to = segment.stops.at(segment.stops.size() - 1);
		const auto& bus = bus_schedule.GetBus(segment.bus);
		std::string start = bus.stops[0];
		std::string finish = bus.is_roundtrip ? "" : bus.stops[(bus.stops.size() - 1) / 2];
		if (from == start || from == finish) {
			Svg::Color color = _parameters.color_palette[context.GetBusIndex(segment.bus) % _parameters.color_palette.size()];
			DrawBusLabel(document, context.GetStopPosition(from), color, segment.bus);
		}
		if (to == start || to == finish) {
			Svg::Color color = _parameters.color_palette[context.GetBusIndex(segment.bus) % _parameters.color_palette.size()];
			DrawBusLabel(document, context.GetStopPosition(std::string(to)), color, segment.bus);
		}
	}
}
