#include "descriptions.h"
#include "json.h"
#include "requests.h"
#include "sphere.h"
#include "transport_catalog.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, const char* argv[]) {

    if (argc != 2) {
        cerr << "Usage: transport_catalog_part_o [make_base|process_requests]\n";
        return 5;
    }

    const string_view mode(argv[1]);
    const auto input_doc = Json::Load(cin);
    const auto& input_map = input_doc.GetRoot().AsMap();
    std::string file = input_map.at("serialization_settings").AsMap().at("file").AsString();
    if (mode == "make_base") {
        const TransportCatalog db(
            Descriptions::ReadDescriptions(input_map.at("base_requests").AsArray()),
            input_map.at("routing_settings").AsMap(),
            input_map.at("render_settings").AsMap(),
            input_map.at("yellow_pages").AsMap()
        );
        std::ofstream out(file, std::ios_base::binary);
        db.Serialize(out);
        out.close();
    }
    else if (mode == "process_requests") {

        std::ifstream in(file, std::ios_base::binary);
        const TransportCatalog db(in);
        in.close();
        Json::PrintValue(
            Requests::ProcessAll(db, input_map.at("stat_requests").AsArray()),
            cout
        );
        cout << endl;
    }
    return 0;
}
