#include "text_parser.h"

IO::TextParser::TextParser(std::istream& input, bool skip_count) : _input(input), _skip_count(skip_count)
{
}

std::shared_ptr<Operations::IUpdateOperation> IO::TextParser::CreateUpdate(std::string&& query)
{
    std::pair<std::string_view, std::string_view> p = Utils::SplitTwo(query);
    if (p.first == "Stop") {
        Models::StopInfo s;
        Parse(s, p.second);
        return std::make_shared<Operations::AddStop>(s);
    }
    else if (p.first == "Bus") {
        Models::BusInfo b;
        Parse(b, p.second);
        return std::make_shared<Operations::AddBus>(b);
    }
    throw std::runtime_error("unknown update operation");
}

std::shared_ptr<Operations::IReadOperation> IO::TextParser::CreateRead(std::string&& query)
{
    std::pair<std::string_view, std::string_view> p = Utils::SplitTwo(query);
    if (p.first == "Bus") {
        return std::make_shared<Operations::BusStat>(std::string(p.second));
    }
    else if (p.first == "Stop") {
        return std::make_shared<Operations::StopStat>(std::string(p.second));
    }
    throw std::runtime_error("unknown read operation");
}

void IO::TextParser::Parse(Models::StopInfo& info, std::string_view view)
{
    auto p1 = Utils::SplitTwo(view, ":");
    info.name = p1.first;
    auto p2 = Utils::SplitTwo(p1.second, ", ");
    info.position.lat = Utils::ToRadians(Utils::ConvertToDouble(p2.first));
    auto p3 = Utils::SplitTwo(p2.second, ", ");
    info.position.lon = Utils::ToRadians(Utils::ConvertToDouble(p3.first));
    if (p3.second != "") {
        std::vector<std::string_view> distances = Utils::SplitBy(p3.second, ", ");
        for (const std::string_view& d : distances) {
            auto p = Utils::SplitTwo(d, "m to ");
            info.known_distance[std::string(p.second)] = Utils::ConvertToDouble(p.first);
        }
    }
}

void IO::TextParser::Parse(Models::BusInfo& info, std::string_view view)
{
    auto p1 = Utils::SplitTwo(view, ": ");
    info.name = p1.first;
    bool is_circle = p1.second.find(">") != p1.second.npos;
    std::string delimiter;
    delimiter = is_circle ? " > " : " - ";
    std::vector<std::string_view> stops_data = Utils::SplitBy(p1.second, delimiter);
    for (const std::string_view& s : stops_data) {
        info.stops.push_back(std::string(s));
    }
    if (!is_circle && stops_data.size() > 1) {
        size_t read_index = stops_data.size() - 2;
        while (true)
        {
            info.stops.push_back(std::string(stops_data.at(read_index)));
            if (read_index == 0) {
                break;
            }
            --read_index;
        }
    }
}

void IO::TextParser::Parse()
{
    size_t q = 0;
    std::string query;
    _input >> q;
    std::getline(_input, query, '\n');
    while (q > 0)
    {
        std::getline(_input, query, '\n');
        _update.push_back(CreateUpdate(std::move(query)));
        --q;
    }

    q = 0;
    _input >> q;
    std::getline(_input, query, '\n');
    while (q > 0)
    {
        std::getline(_input, query, '\n');
        _read.push_back(CreateRead(move(query)));
        --q;
    }
}

