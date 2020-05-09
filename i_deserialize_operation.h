#pragma once

#include "bus_schedule.h"

#include <iostream>

namespace Operations {
	class IDeserializeOperation {
	public:
		virtual void Deserialize(Catalog::BusShedule& bus_shedule) = 0;
		virtual ~IDeserializeOperation() {};
	};
}
