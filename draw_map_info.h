#pragma once

#include "document.h"

#include <string>
#include <memory>

namespace Models {
	struct DrawMapInfo {
		size_t request_id;
		std::shared_ptr<Svg::Document> svg;
	};
}