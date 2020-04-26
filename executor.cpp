#include "executor.h"

void Executor::Process(IO::IFormatter& formatter, IO::Parser& parser, Catalog::BusShedule& schedule)
{
    parser.Parse();
    for (const auto q : parser.GetUpdate()) {
        q->Do(schedule);
    }
    for (const auto& q : parser.GetRead()) {
        q->Do(schedule, formatter);
    }
    formatter.Flush();
}

void Executor::Process(IO::IFormatter& formatter, IO::Parser& parser, Catalog::BusShedule& schedule, Catalog::MapBuilder& map_builder)
{
    parser.Parse();
    for (const auto q : parser.GetUpdate()) {
        q->Do(schedule);
    }
    for (const auto& q : parser.GetRead()) {
        q->Do(schedule, formatter);
    }
    std::cerr << "readed" << std::endl;
    for (const auto& q : parser.GetMapBuilderSetup()) {
        q->Do(map_builder);
    }
    std::cerr << "builder set up" << std::endl;
    for (const auto& q : parser.GetDrawMap()) {
        q->DrawMap(schedule, map_builder, formatter);
    }
    std::cerr << "drawed" << std::endl;
    formatter.Flush();
}
