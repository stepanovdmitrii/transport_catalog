#pragma once

#include "bus_schedule.h"
#include "map_builder.h"
#include "i_formatter.h"

namespace Operations {
	class IDrawMapOperation {
	public:
		virtual void DrawMap(Catalog::BusShedule& bus_schedule, Catalog::MapBuilder& map_builder, IO::IFormatter& formatter) = 0;
		virtual ~IDrawMapOperation();
	};
}
