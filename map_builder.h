#pragma once

#include "map_builder_parameters.h"
#include "bus_schedule.h"
#include "document.h"
#include "comp_context.h"
#include "multi_stop_projection.h"
#include "i_stop_projection.h"
#include "compress_coordinates_projection.h"
#include "uniform_distribution_operation.h"

#include <string>
#include <memory>
#include <cmath>
#include <limits>
#include <unordered_map>

namespace Catalog {
	class MapBuilder
	{
	public:
		MapBuilder() = default;

		void Setup(const Models::MapBuilderParameters& params);
		std::unique_ptr<Svg::Document> DrawSvg(const Catalog::BusShedule& bus_schedule) const;
	private:
		Models::MapBuilderParameters _parameters;

		std::unique_ptr<Catalog::ComputationContext> GetContext(const Catalog::BusShedule& bus_schedule) const;
		void DrawBusLines(const Catalog::BusShedule& bus_schedule, Svg::Document& document, const Catalog::ComputationContext& context) const;
		void DrawStops(const Catalog::BusShedule& bus_schedule, Svg::Document& document, const Catalog::ComputationContext& context) const;
		void DrawStopLabels(const Catalog::BusShedule& bus_schedule, Svg::Document& document, const Catalog::ComputationContext& context) const;
		void DrawBusLabels(const Catalog::BusShedule& bus_schedule, Svg::Document& document, const Catalog::ComputationContext& context) const;
		void DrawBusLabel(Svg::Document& document, Svg::Point position, const Svg::Color& color, const std::string& name) const;
	};
}


