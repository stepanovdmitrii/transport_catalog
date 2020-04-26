#pragma once

#include <vector>
#include <string>

namespace Models {
	struct Bus {
		bool is_roundtrip;
		std::vector<std::string> stops;
	};
}