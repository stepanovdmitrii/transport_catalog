#pragma once

#include <unordered_set>
#include <string>
#include <vector>

#include "bus_schedule.h"
#include "i_stop_projection.h"

namespace Catalog {
	class UniformDistributionOperation : public IStopProjection
	{
	public:
		UniformDistributionOperation();

		virtual void Project(const BusShedule& bus_schedule, std::unordered_map<std::string, Models::Stop>& stops) override;

	private:

	};
}

