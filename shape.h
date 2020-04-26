#pragma once

#include "color.h"
#include "i_object.h"

#include <iostream>
#include <string>
#include <optional>

namespace Svg {
	template <typename TShape>
	class Shape
	{
	public:
		Shape() : _fill(NoneColor), _stroke(NoneColor), _stroke_width(1.0), _stroke_linecap(std::nullopt), _stroke_linejoin(std::nullopt) {

		}

		TShape& SetFillColor(const Color& value) {
			_fill = value;
			return This();
		}
		TShape& SetStrokeColor(const Color& value) {
			_stroke = value;
			return This();
		}
		TShape& SetStrokeWidth(double value) {
			_stroke_width = value;
			return This();
		}
		TShape& SetStrokeLineCap(const std::string& value) {
			_stroke_linecap = value;
			return This();
		}
		TShape& SetStrokeLineJoin(const std::string& value) {
			_stroke_linejoin = value;
			return This();
		}
	protected:
		void RenderCommon(std::ostream& output) {
			output << "fill=\"" << _fill.ToSvg() << "\" ";
			output << "stroke=\"" << _stroke.ToSvg() << "\" ";
			output << "stroke-width=\"" << _stroke_width << "\" ";
			if (_stroke_linecap.has_value()) {
				output << "stroke-linecap=\"" << *_stroke_linecap << "\" ";
			}
			if (_stroke_linejoin.has_value()) {
				output << "stroke-linejoin=\"" << *_stroke_linejoin << "\" ";
			}
		}
	private:
		Color _fill;
		Color _stroke;
		double _stroke_width;
		std::optional<std::string> _stroke_linecap;
		std::optional<std::string> _stroke_linejoin;
		
		TShape& This() {
			return static_cast<TShape&>(*this);
		}
		
	};
}

