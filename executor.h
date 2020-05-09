#pragma once

#include "i_formatter.h"
#include "parser.h"
#include "bus_schedule.h"
#include "i_update_operation.h"
#include "i_read_operation.h"
#include "map_builder.h"
#include "i_map_builder_setup_operation.h"
#include "i_draw_map_operation.h"

#include <vector>
#include <memory>
#include <exception>
#include <iostream>
#include <string>

class Executor
{
public:
	static void Process(IO::IFormatter& formatter, IO::Parser& parser, Catalog::BusShedule& schedule);
	static void ProcessMakeBase(IO::IFormatter& formatter, IO::Parser& parser, Catalog::BusShedule& schedule, Catalog::MapBuilder& map_builder);
	static void ProcessRequests(IO::IFormatter& formatter, IO::Parser& parser, Catalog::BusShedule& schedule, Catalog::MapBuilder& map_builder);
};

