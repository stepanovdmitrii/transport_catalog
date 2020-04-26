#include "map_builder_set_parameters.h"

void Operations::MapBuilderSetParameters::Do(Catalog::MapBuilder& map_builder)
{
	map_builder.Setup(_params);
}

Operations::MapBuilderSetParameters& Operations::MapBuilderSetParameters::SetWidth(double width)
{
	_params.width = width;
	return *this;
}

Operations::MapBuilderSetParameters& Operations::MapBuilderSetParameters::SetHeight(double height)
{
	_params.height = height;
	return *this;
}

Operations::MapBuilderSetParameters& Operations::MapBuilderSetParameters::SetPadding(double padding)
{
	_params.padding = padding;
	return *this;
}

Operations::MapBuilderSetParameters& Operations::MapBuilderSetParameters::SetStopRadius(double stop_radius)
{
	_params.stop_radius = stop_radius;
	return *this;
}

Operations::MapBuilderSetParameters& Operations::MapBuilderSetParameters::SetLineWidth(double line_width)
{
	_params.line_width = line_width;
	return *this;
}

Operations::MapBuilderSetParameters& Operations::MapBuilderSetParameters::SetStopLabelSize(uint32_t stop_label_size)
{
	_params.stop_label_font_size = stop_label_size;
	return *this;
}

Operations::MapBuilderSetParameters& Operations::MapBuilderSetParameters::SetStopLabelOffset(double x, double y)
{
	_params.stop_label_offset.x = x;
	_params.stop_label_offset.y = y;
	return *this;
}

Operations::MapBuilderSetParameters& Operations::MapBuilderSetParameters::SetBusLabelSize(uint32_t bus_label_size)
{
	_params.bus_label_font_size = bus_label_size;
	return *this;
}

Operations::MapBuilderSetParameters& Operations::MapBuilderSetParameters::SetBusLabelOffset(double x, double y)
{
	_params.bus_label_offset.x = x;
	_params.bus_label_offset.y = y;
	return *this;
}

Operations::MapBuilderSetParameters& Operations::MapBuilderSetParameters::SetUnderlayerWidth(double underlayer_width)
{
	_params.underlayer_width = underlayer_width;
	return *this;
}

Operations::MapBuilderSetParameters& Operations::MapBuilderSetParameters::SetUnderlayerColor(const std::string& color)
{
	_params.underlayer_color = color;
	return *this;
}

Operations::MapBuilderSetParameters& Operations::MapBuilderSetParameters::SetUnderlayerColor(std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
	_params.underlayer_color = Svg::Rgb(r, g, b);
	return *this;
}

Operations::MapBuilderSetParameters& Operations::MapBuilderSetParameters::SetUnderlayerColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, double a)
{
	_params.underlayer_color = Svg::Rgba(r, g, b, a);
	return *this;
}

Operations::MapBuilderSetParameters& Operations::MapBuilderSetParameters::AddColorToPalette(const std::string& color)
{
	_params.color_palette.push_back(color);
	return *this;
}

Operations::MapBuilderSetParameters& Operations::MapBuilderSetParameters::AddColorToPalette(std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
	_params.color_palette.push_back(Svg::Rgb(r, g, b));
	return *this;
}

Operations::MapBuilderSetParameters& Operations::MapBuilderSetParameters::AddColorToPalette(std::uint8_t r, std::uint8_t g, std::uint8_t b, double a)
{
	_params.color_palette.push_back(Svg::Rgba(r, g, b, a));
	return *this;
}

Operations::MapBuilderSetParameters& Operations::MapBuilderSetParameters::AddLayer(const std::string& layer)
{
	_params.layers.push_back(layer);
	return *this;
}
