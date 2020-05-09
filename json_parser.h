#pragma once

#include "parser.h"
#include "i_update_operation.h"
#include "i_read_operation.h"
#include "add_bus_operation.h"
#include "add_stop_operation.h"
#include "find_way_operation.h"
#include "set_route_parameters_operation.h"
#include "bus_stat_operation.h"
#include "stop_stat_operation.h"
#include "json.h"
#include "bus_info.h"
#include "stop_info.h"
#include "utils.h"
#include "map_builder_set_parameters.h"
#include "i_map_builder_setup_operation.h"
#include "i_draw_map_operation.h"
#include "draw_map_operation.h"
#include "mode.h"
#include "ProtoBufDeserializer.h"
#include "ProtoBufSerializer.h"

#include <memory>
#include <iostream>
#include <optional>
#include <vector>
#include <string>
#include <exception>
#include <map>

namespace IO {
    class JsonParser : public Parser {
    public:
        JsonParser(std::istream& input, OperationMode mode);
        virtual void Parse() override;

    private:
        std::istream& _input;
        OperationMode _mode;

        void ParseRouteSettings(Json::Document& doc);
        void ParseBaseRequests(Json::Document& doc);
        void ParseStatRequests(Json::Document& doc);
        void ParseRenderSettings(Json::Document& doc);
        void ParseSerializeOperations(Json::Document& doc);

        std::shared_ptr<Operations::AddStop> ParseAddStop(const std::map<std::string, Json::Node>& stop_node);
        std::shared_ptr<Operations::AddBus> ParseAddBus(const std::map<std::string, Json::Node>& bus_node);
    };
}

