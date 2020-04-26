#pragma once

#include "position.h"
#include "point.h"
#include "stop.h"

#include <memory>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <unordered_map>

namespace Catalog {
	class ComputationContext
	{
	public:
		ComputationContext(std::unordered_map<std::string,Models::Stop>&& stops);

		Svg::Point GetStopPosition(const std::string& stop) const;
	private:
		std::unordered_map<std::string, Models::Stop> _stops;
	};
}

