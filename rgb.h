#pragma once

#include <cstdint>
#include <string>
#include <sstream>

namespace Svg {
	struct Rgb
	{
		Rgb();
		Rgb(std::uint8_t, std::uint8_t, std::uint8_t);

		uint8_t red;
		uint8_t green;
		uint8_t blue;

		std::string ToSvg() const;
	};
}

