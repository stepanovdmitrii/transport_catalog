#pragma once

#include <unordered_map>
#include <set>
#include <string>
#include <vector>

#include "i_stop_projection.h"
#include "map_builder_parameters.h"
#include "stop.h"

namespace Catalog {
	class CompressCoordinatesProjection: public IStopProjection
	{
	public:
		CompressCoordinatesProjection(Models::MapBuilderParameters parameters);
		// Унаследовано через IStopProjection
		virtual void Project(const BusShedule& bus_schedule, std::unordered_map<std::string, Models::Stop>& stops) override;
	private:
		Models::MapBuilderParameters _parameters;

		bool AreNeighborsOnSameRoute(const Catalog::BusShedule& bus_schedule, const Models::Stop& first, const Models::Stop& second) const;
		std::vector<Models::Stop> RemoveNeighbors(const Catalog::BusShedule& bus_schedule, std::vector<Models::Stop>& ordered) const;
	};
}

