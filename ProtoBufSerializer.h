#pragma once

#include "i_serialize_operation.h"
#include "bus_schedule.h"
#include "transport_catalog.pb.h"

#include <string>
#include <iostream>
#include <fstream>

namespace Operations {
	class ProtoBufSerializer: public ISerializeOperation
	{
	public:
		ProtoBufSerializer(std::string file);

		// Унаследовано через ISerializeOperation
		virtual void Serialize(const Catalog::BusShedule& bus_schedule) override;

	private:
		std::string _file;
	};
}

