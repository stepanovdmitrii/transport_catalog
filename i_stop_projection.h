#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "stop.h"
#include "bus_schedule.h"

namespace Catalog {
	class IStopProjection {
	public:
		virtual void Project(const BusShedule& bus_schedule, std::unordered_map<std::string, Models::Stop>& stops) = 0;
		virtual ~IStopProjection();
	};
}
