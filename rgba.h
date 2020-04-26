#pragma once

#include <cstdint>
#include <string>
#include <sstream>

namespace Svg {
	struct Rgba
	{
		Rgba();
		Rgba(std::uint8_t, std::uint8_t, std::uint8_t, double);

		uint8_t red;
		uint8_t green;
		uint8_t blue;
		double alpha;

		std::string ToSvg() const;
	};
}

