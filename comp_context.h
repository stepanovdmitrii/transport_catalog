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
#include <iostream>

namespace Catalog {
	class ComputationContext
	{
	public:
		ComputationContext(std::unordered_map<std::string,Models::Stop>&& stops);

		Svg::Point GetStopPosition(const std::string& stop) const;
		void SetBusIndex(const std::string& bus, size_t index);
		size_t GetBusIndex(const std::string& bus) const;
	private:
		std::unordered_map<std::string, Models::Stop> _stops;
		std::unordered_map<std::string, size_t> _bus_indexes;
	};
}

