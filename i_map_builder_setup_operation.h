#pragma once

#include "map_builder.h"

namespace Operations {
	class IMapBuilderSetupOperation {
	public:
		virtual void Do(Catalog::MapBuilder& map_builder) = 0;
		virtual ~IMapBuilderSetupOperation();
	};
}