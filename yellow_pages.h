#pragma once

#include "transport_catalog.pb.h"
#include "json.h"

#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

struct CompanyInfo {
    std::string company_name;
};

struct PhoneQuery {
    std::string type;
    std::string country_code;
    std::string local_code;
    std::string number;
    std::string extension;

    bool DoesPhoneMatch(const serialization::Phone& object) const;

private:
    serialization::Phone_Type phone_type() const;
};

struct CompanyQuery {
    std::unordered_set<std::string> names;
    std::unordered_set<std::string> urls;
    std::unordered_set<std::string> rubrics;
    std::vector<PhoneQuery> phones;

    static CompanyQuery Create(const Json::Dict& attrs);
};

class YellowPages
{
public:
	YellowPages(const Json::Dict& yellow_pages_json);
    YellowPages(serialization::Database&& database);

    std::vector<CompanyInfo> FindCompanies(const CompanyQuery& query) const;
    void Serialize(serialization::Database& catalog) const;
private:
	serialization::Database _database;

    void ParseCompanies(const Json::Dict& yellow_pages_json);
    void ParseRubrics(const Json::Dict& yellow_pages_json);
    void ParseCompanyNearbyStops(serialization::Company& comp, const Json::Dict& company_dict);
    void ParseCompanyRubrics(serialization::Company& comp, const Json::Dict& company_dict);
    void ParseCompanyUrls(serialization::Company& comp, const Json::Dict& company_dict);
    void ParseCompanyAddress(serialization::Company& comp, const Json::Dict& company_dict);
    void ParseCompanyNames(serialization::Company& comp, const Json::Dict& company_dict);
    void ParseCompanyPhones(serialization::Company& comp, const Json::Dict& company_dict);
};

