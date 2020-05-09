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

void Executor::ProcessMakeBase(IO::IFormatter& formatter, IO::Parser& parser, Catalog::BusShedule& schedule, Catalog::MapBuilder& map_builder)
{
    parser.Parse();
    for (const auto& q : parser.GetUpdate()) {
        q->Do(schedule);
    }
    for (const auto& s : parser.GetSerialize()) {
        s->Serialize(schedule);
    }
}

void Executor::ProcessRequests(IO::IFormatter& formatter, IO::Parser& parser, Catalog::BusShedule& schedule, Catalog::MapBuilder& map_builder)
{
    parser.Parse();
    for (const auto& d : parser.GetDeserialize()) {
        d->Deserialize(schedule);
    }
    for (const auto& q : parser.GetRead()) {
        q->Do(schedule, formatter);
    }
    for (const auto& q : parser.GetMapBuilderSetup()) {
        q->Do(map_builder);
    }
    for (const auto& q : parser.GetDrawMap()) {
        q->DrawMap(schedule, map_builder, formatter);
    }
    formatter.Flush();
}
