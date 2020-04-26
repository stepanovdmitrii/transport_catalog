#pragma once

#include "bus_schedule.h"
#include "i_formatter.h"

namespace Operations {
	class IReadOperation {
	public:
		virtual void Do(Catalog::BusShedule& bus_schedule, IO::IFormatter& formatter) = 0;
		virtual ~IReadOperation();
	};
}