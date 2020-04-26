#pragma once

#include "i_map_builder_setup_operation.h"
#include "map_builder_parameters.h"
#include "rgb.h"
#include "rgba.h"

#include <string>
#include <cstdint>

namespace Operations {
	class MapBuilderSetParameters: public IMapBuilderSetupOperation
	{
	public:
		MapBuilderSetParameters() = default;

		MapBuilderSetParameters& SetWidth(double width);
		MapBuilderSetParameters& SetHeight(double height);
		MapBuilderSetParameters& SetPadding(double padding);
		MapBuilderSetParameters& SetStopRadius(double stop_radius);
		MapBuilderSetParameters& SetLineWidth(double line_width);
		MapBuilderSetParameters& SetStopLabelSize(uint32_t stop_label_size);
		MapBuilderSetParameters& SetStopLabelOffset(double x, double y);
		MapBuilderSetParameters& SetBusLabelSize(uint32_t bus_label_size);
		MapBuilderSetParameters& SetBusLabelOffset(double x, double y);
		MapBuilderSetParameters& SetUnderlayerWidth(double underlayer_width);
		MapBuilderSetParameters& SetUnderlayerColor(const std::string& color);
		MapBuilderSetParameters& SetUnderlayerColor(std::uint8_t r, std::uint8_t g, std::uint8_t b);
		MapBuilderSetParameters& SetUnderlayerColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, double a);
		MapBuilderSetParameters& AddColorToPalette(const std::string& color);
		MapBuilderSetParameters& AddColorToPalette(std::uint8_t r, std::uint8_t g, std::uint8_t b);
		MapBuilderSetParameters& AddColorToPalette(std::uint8_t r, std::uint8_t g, std::uint8_t b, double a);
		MapBuilderSetParameters& AddLayer(const std::string& layer);

		virtual void Do(Catalog::MapBuilder& map_builder) override;
	private:
		Models::MapBuilderParameters _params;
	};
}

