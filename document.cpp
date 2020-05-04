#include "document.h"

Svg::Document::Document()
{
}

void Svg::Document::Add(const Circle& circle)
{
	_shapes.push_back(std::move(std::make_unique<Svg::Circle>(circle)));
}

void Svg::Document::Add(const Polyline& polyline)
{
	_shapes.push_back(std::move(std::make_unique<Svg::Polyline>(polyline)));
}

void Svg::Document::Add(const Text& text)
{
	_shapes.push_back(std::move(std::make_unique<Svg::Text>(text)));
}

void Svg::Document::Add(Svg::IObject&& shape)
{
	std::unique_ptr<Svg::IObject> ptr(&shape);
	_shapes.push_back(std::move(ptr));
}

void Svg::Document::Add(const Rectangle& rect)
{
	_shapes.push_back(std::move(std::make_unique<Svg::Rectangle>(rect)));
}

void Svg::Document::Render(std::ostream& output)
{
	output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	output << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">";
	for (const auto& shape : _shapes) {
		shape->Render(output);
	}
	output << "</svg>";
}

