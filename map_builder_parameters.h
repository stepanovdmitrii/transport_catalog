#pragma once

#include "point.h"
#include "color.h"

#include <cstdint>
#include <vector>

namespace Models {
	struct MapBuilderParameters {
		double width;
		double height;
		double padding;
		double stop_radius;
		double line_width;
		double outer_margin;
		std::uint32_t stop_label_font_size;
		std::uint32_t bus_label_font_size;
		Svg::Point stop_label_offset;
		Svg::Point bus_label_offset;
		Svg::Color underlayer_color;
		double underlayer_width;
		std::vector<Svg::Color> color_palette;
		std::vector<std::string> layers;
	};
}
