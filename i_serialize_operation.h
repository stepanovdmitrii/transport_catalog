#pragma once

#include "bus_schedule.h"
#include "stop.h"

#include <unordered_map>
#include <string>

namespace Operations {
	class ISerializeOperation
	{
	public:
		virtual void Serialize(const Catalog::BusShedule& bus_schedule) = 0;
		virtual ~ISerializeOperation() {};
	};
}

