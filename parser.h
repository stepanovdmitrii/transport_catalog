#pragma once

#include "i_read_operation.h"
#include "i_update_operation.h"
#include "i_draw_map_operation.h"
#include "i_map_builder_setup_operation.h"
#include "range.h"

#include <vector>
#include <memory>

namespace IO {
    class Parser {
    public:
        using UpdateIterator = std::vector<std::shared_ptr<Operations::IUpdateOperation>>::iterator;
        using ReadIterator = std::vector<std::shared_ptr<Operations::IReadOperation>>::iterator;
        using MapBuilderSetupIterator = std::vector<std::shared_ptr<Operations::IMapBuilderSetupOperation>>::iterator;
        using DrawMapIterator = std::vector<std::shared_ptr<Operations::IDrawMapOperation>>::iterator;

        virtual void Parse() = 0;

        Range<UpdateIterator> GetUpdate();
        Range<ReadIterator> GetRead();
        Range<MapBuilderSetupIterator> GetMapBuilderSetup();
        Range<DrawMapIterator> GetDrawMap();
        virtual ~Parser();
    protected:
        std::vector<std::shared_ptr<Operations::IUpdateOperation>> _update;
        std::vector<std::shared_ptr<Operations::IReadOperation>> _read;
        std::vector<std::shared_ptr<Operations::IMapBuilderSetupOperation>> _builder_setup;
        std::vector<std::shared_ptr<Operations::IDrawMapOperation>> _draw_map;
    };
}