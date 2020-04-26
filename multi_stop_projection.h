#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include "bus_schedule.h"
#include "i_stop_projection.h"

namespace Catalog {
	class MultiStopProjection : public IStopProjection
	{
	public:
		MultiStopProjection(std::vector<std::shared_ptr<IStopProjection>> projections);

		// Унаследовано через IStopProjection
		virtual void Project(const BusShedule& bus_schedule, std::unordered_map<std::string, Models::Stop>& stops) override;
	private:
		std::vector<std::shared_ptr<IStopProjection>> _projections;
	};
}

