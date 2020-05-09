#pragma once

#include "i_deserialize_operation.h"
#include "bus_schedule.h"
#include "transport_catalog.pb.h"
#include "bus_info.h"
#include "stop_info.h"

#include <string>
#include <iostream>
#include <fstream>

namespace Operations {
	class ProtoBufDeserializer: public IDeserializeOperation
	{
	public:
		ProtoBufDeserializer(std::string file);

		// Унаследовано через IDeserializeOperation
		virtual void Deserialize(Catalog::BusShedule& bus_shedule) override;

	private:
		std::string _file;
	};
}

