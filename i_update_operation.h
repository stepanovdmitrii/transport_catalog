#pragma once

#include "bus_schedule.h"

namespace Operations {
	class IUpdateOperation {
	public:
		virtual void Do(Catalog::BusShedule& bus_schedule) = 0;
		virtual ~IUpdateOperation();
	};
}