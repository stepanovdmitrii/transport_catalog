#include "test_fixture.h"
#include "bus_schedule.h"
#include "json_formatter.h"
#include "json_parser.h"

#include <iostream>

int main() {
    UnitTests::TestFixture::Run();

    Catalog::BusShedule schedule;
    Catalog::MapBuilder map_builder;
    IO::JsonFormatter formatter(std::cout);
    IO::JsonParser parser(std::cin);
    Executor::Process(formatter, parser, schedule, map_builder);

    return 0;
}