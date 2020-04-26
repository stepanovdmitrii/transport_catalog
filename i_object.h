#pragma once

#include <iostream>

namespace Svg {
    class IObject {
    public:
        virtual void Render(std::ostream&) = 0;
        virtual ~IObject();
    };
}