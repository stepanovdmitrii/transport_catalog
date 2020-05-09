#include "test_fixture.h"
#include "bus_schedule.h"
#include "json_formatter.h"
#include "json_parser.h"
#include "mode.h"

#include <iostream>
#include <string_view>

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: transport_catalog_part_o [make_base|process_requests]\n";
        return 5;
    }
    OperationMode mode_value = OperationMode::MAKE_BASE;
    const std::string_view mode(argv[1]);
    if (mode == "process_requests") {

        mode_value = OperationMode::PROCESS_REQUESTS;

    }
    UnitTests::TestFixture::Run();

    Catalog::BusShedule schedule;
    Catalog::MapBuilder map_builder;
    IO::JsonFormatter formatter(std::cout);
    IO::JsonParser parser(std::cin, mode_value);
    if (mode_value == OperationMode::MAKE_BASE) {
        Executor::ProcessMakeBase(formatter, parser, schedule, map_builder);
    }
    else if (mode_value == OperationMode::PROCESS_REQUESTS) {
        Executor::ProcessRequests(formatter, parser, schedule, map_builder);
    }
    
    



    return 0;
}