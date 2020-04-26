#pragma once
#include "shape.h"
#include "i_object.h"
#include "circle.h"
#include "polyline.h"
#include "text.h"

#include <iostream>
#include <vector>
#include <memory>

namespace Svg {
	class Document : public IObject
	{
	public:
		Document();

		void Add(const Circle&);
		void Add(const Polyline&);
		void Add(const Text&);
		void Add(IObject&&);

		virtual void Render(std::ostream&) override;
	private:
		std::vector<std::unique_ptr<IObject>> _shapes;
	};
}

