#include "test_fixture.h"

void UnitTests::TestFixture::Run()
{
	TestRunner tr;
	RUN_TEST(tr, RunTests);
}

void UnitTests::TestFixture::RunTests()
{
	PerformOperationsText();
	//PerformOperationsJson();
    SvgTest();
}

void UnitTests::TestFixture::PerformOperationsText()
{
    std::vector<std::string> update = {
    "Stop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino",
    "Stop Marushkino: 55.595884, 37.209755, 9900m to Rasskazovka",
    "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye",
    "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka",
    "Stop Rasskazovka: 55.632761, 37.333324",
    "Stop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam",
    "Stop Biryusinka: 55.581065, 37.64839, 750m to Universam",
    "Stop Universam: 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya",
    "Stop Biryulyovo Tovarnaya: 55.592028, 37.653656, 1300m to Biryulyovo Passazhirskaya",
    "Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164, 1200m to Biryulyovo Zapadnoye",
    "Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye",
    "Stop Rossoshanskaya ulitsa: 55.595579, 37.605757",
    "Stop Prazhskaya: 55.611678, 37.603831"
    };

    std::vector<std::string> read = {
        "Bus 256",
        "Bus 750",
        "Bus 751",
        "Stop Samara",
        "Stop Prazhskaya",
        "Stop Biryulyovo Zapadnoye"
    };

    std::string expected = "Bus 256: 6 stops on route, 5 unique stops, 5950 route length, 1.36124 curvature\nBus 750: 5 stops on route, 3 unique stops, 27600 route length, 1.31808 curvature\nBus 751: not found\nStop Samara: not found\nStop Prazhskaya: no buses\nStop Biryulyovo Zapadnoye: buses 256 828\n";

    Catalog::BusShedule schedule;
    std::stringstream input;
    std::stringstream output;
    input << update.size() << std::endl;
    for (const std::string& s : update) {
        input << s << std::endl;
    }
    input << read.size() << std::endl;
    for (const std::string& s : read) {
        input << s << std::endl;
    }

    IO::TextParser p(input);
    IO::TextFormatter f(output);
    Executor::Process(f, p, schedule);

    ASSERT_EQUAL(expected, output.str());
}

void UnitTests::TestFixture::PerformOperationsJson()
{
    std::string input = "{\"routing_settings\":{\"bus_wait_time\":6,\"bus_velocity\":40},\"base_requests\":[{\"type\":\"Bus\",\"name\":\"297\",\"stops\":[\"Biryulyovo Zapadnoye\",\"Biryulyovo Tovarnaya\",\"Universam\",\"Biryulyovo Zapadnoye\"],\"is_roundtrip\":true},{\"type\":\"Bus\",\"name\":\"635\",\"stops\":[\"Biryulyovo Tovarnaya\",\"Universam\",\"Prazhskaya\"],\"is_roundtrip\":false},{\"type\":\"Stop\",\"road_distances\":{\"Biryulyovo Tovarnaya\":2600},\"longitude\":37.6517,\"name\":\"Biryulyovo Zapadnoye\",\"latitude\":55.574371},{\"type\":\"Stop\",\"road_distances\":{\"Prazhskaya\":4650,\"Biryulyovo Tovarnaya\":1380,\"Biryulyovo Zapadnoye\":2500},\"longitude\":37.645687,\"name\":\"Universam\",\"latitude\":55.587655},{\"type\":\"Stop\",\"road_distances\":{\"Universam\":890},\"longitude\":37.653656,\"name\":\"Biryulyovo Tovarnaya\",\"latitude\":55.592028},{\"type\":\"Stop\",\"road_distances\":{},\"longitude\":37.603938,\"name\":\"Prazhskaya\",\"latitude\":55.611717}],\"stat_requests\":[{\"type\":\"Bus\",\"name\":\"297\",\"id\":1},{\"type\":\"Bus\",\"name\":\"635\",\"id\":2},{\"type\":\"Stop\",\"name\":\"Universam\",\"id\":3},{\"type\":\"Route\",\"from\":\"Biryulyovo Zapadnoye\",\"to\":\"Universam\",\"id\":4},{\"type\":\"Route\",\"from\":\"Biryulyovo Zapadnoye\",\"to\":\"Prazhskaya\",\"id\":5}]}";
    std::string expected = "[{\"route_length\":5990,\"request_id\":1,\"curvature\":1.42963,\"stop_count\":4,\"unique_stop_count\":3},{\"route_length\":11570,\"request_id\":2,\"curvature\":1.30156,\"stop_count\":5,\"unique_stop_count\":3},{\"buses\":[\"297\",\"635\"],\"request_id\":3},{\"request_id\":4,\"total_time\":11.235,\"items\":[{\"type\":\"Wait\",\"stop_name\":\"Biryulyovo Zapadnoye\",\"time\":6},{\"type\":\"Bus\",\"bus\":\"297\",\"span_count\":2,\"time\":5.235}]},{\"request_id\":5,\"total_time\":24.21,\"items\":[{\"type\":\"Wait\",\"stop_name\":\"Biryulyovo Zapadnoye\",\"time\":6},{\"type\":\"Bus\",\"bus\":\"297\",\"span_count\":1,\"time\":3.9},{\"type\":\"Wait\",\"stop_name\":\"Biryulyovo Tovarnaya\",\"time\":6},{\"type\":\"Bus\",\"bus\":\"635\",\"span_count\":2,\"time\":8.31}]}]";
    std::stringstream input_stream(input);
    std::stringstream output_stream;
    IO::JsonParser p(input_stream);
    IO::JsonFormatter f(output_stream);
    Catalog::BusShedule schedule;
    Executor::Process(f, p, schedule);
    ASSERT_EQUAL(expected, output_stream.str());
}

void UnitTests::TestFixture::SvgTest()
{
    const std::string expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\"><polyline points=\"50,50 250,250 \" fill=\"none\" stroke=\"rgb(140,198,63)\" stroke-width=\"16\" stroke-linecap=\"round\" /><circle cx=\"50\" cy=\"50\" r=\"6\" fill=\"white\" stroke=\"none\" stroke-width=\"1\" /><circle cx=\"250\" cy=\"250\" r=\"6\" fill=\"white\" stroke=\"none\" stroke-width=\"1\" /><text x=\"50\" y=\"50\" dx=\"10\" dy=\"-10\" font-size=\"20\" font-family=\"Verdana\" fill=\"black\" stroke=\"none\" stroke-width=\"1\" >C</text><text x=\"250\" y=\"250\" dx=\"10\" dy=\"-10\" font-size=\"20\" font-family=\"Verdana\" fill=\"black\" stroke=\"none\" stroke-width=\"1\" >C++</text></svg>";

    std::stringstream out;
    Svg::Document svg;

    svg.Add(std::move(
        Svg::Polyline{}
        .SetStrokeColor(Svg::Rgb{ 140, 198, 63 })  // soft green
        .SetStrokeWidth(16)
        .SetStrokeLineCap("round")
        .AddPoint({ 50, 50 })
        .AddPoint({ 250, 250 }))
    );

    for (const auto point : { Svg::Point{50, 50}, Svg::Point{250, 250} }) {
        svg.Add(std::move(
            Svg::Circle{}
            .SetFillColor("white")
            .SetRadius(6)
            .SetCenter(point))
        );
    }

    svg.Add(std::move(
        Svg::Text{}
        .SetPoint({ 50, 50 })
        .SetOffset({ 10, -10 })
        .SetFontSize(20)
        .SetFontFamily("Verdana")
        .SetFillColor("black")
        .SetData("C"))
    );
    svg.Add(std::move(
        Svg::Text{}
        .SetPoint({ 250, 250 })
        .SetOffset({ 10, -10 })
        .SetFontSize(20)
        .SetFontFamily("Verdana")
        .SetFillColor("black")
        .SetData("C++"))
    );

    svg.Render(out);
    ASSERT_EQUAL(expected, out.str());

}
