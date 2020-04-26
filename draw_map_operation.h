#pragma once
#include "i_draw_map_operation.h"
#include "bus_schedule.h"
#include "map_builder.h"
#include "i_formatter.h"

namespace Operations {
	class DrawMapOperation: public IDrawMapOperation
	{
	public:
		DrawMapOperation(size_t request_id);

		virtual void DrawMap(Catalog::BusShedule& bus_schedule, Catalog::MapBuilder& map_builder, IO::IFormatter& formatter) override;
	private:
		size_t _request_id;
	};
}

