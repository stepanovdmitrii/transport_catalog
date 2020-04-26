#pragma once

#include "parser.h"
#include "i_update_operation.h"
#include "i_read_operation.h"
#include "add_bus_operation.h"
#include "add_stop_operation.h"
#include "utils.h"
#include "stop_info.h"
#include "bus_info.h"
#include "bus_stat_operation.h"
#include "stop_stat_operation.h"

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <string_view>
#include <exception>

namespace IO {
    class TextParser : public Parser {
    public:
        TextParser(std::istream& input, bool skip_count = false);
        
        virtual void Parse() override;

        static std::shared_ptr<Operations::IUpdateOperation> CreateUpdate(std::string&& query);
        static std::shared_ptr<Operations::IReadOperation> CreateRead(std::string&& query);
    private:
        std::istream& _input;
        bool _skip_count;

        static void Parse(Models::StopInfo& info, std::string_view view);
        static void Parse(Models::BusInfo& info, std::string_view view);
    };
}

