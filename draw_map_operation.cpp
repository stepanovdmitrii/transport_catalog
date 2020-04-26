#include "draw_map_operation.h"

Operations::DrawMapOperation::DrawMapOperation(size_t request_id): _request_id(request_id)
{
}

void Operations::DrawMapOperation::DrawMap(Catalog::BusShedule& bus_schedule, Catalog::MapBuilder& map_builder, IO::IFormatter& formatter)
{
	Models::DrawMapInfo info;
	info.svg = std::move(map_builder.DrawSvg(bus_schedule));
	info.request_id = _request_id;
	formatter.Write(info);
}
