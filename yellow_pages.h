#pragma once

#include "transport_catalog.pb.h"
#include "json.h"
#include "descriptions.h"

#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

struct CompanyInfo {
    std::string company_name;
    int index;
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

std::string GetCompanyName(const serialization::Company& company);

class YellowPages
{
public:
	YellowPages(const Json::Dict& yellow_pages_json);
    YellowPages(serialization::Database&& database);

    std::vector<CompanyInfo> FindCompanies(const CompanyQuery& query) const;
    void Serialize(serialization::Database& catalog) const;
    int GetCompaniesCount() const;
    const serialization::Company& GetCompany(int index) const;
    std::string GetRubricName(uint64_t rubric_id) const;
    double GetWaitTime(int index, double arrival_time) const;
private:
	serialization::Database _database;
    std::unordered_map<int, std::vector<std::pair<double, double>>> _working_intervals;
    std::unordered_map<serialization::WorkingTimeInterval_Day, int32_t> _day_to_index;

    void ParseCompanies(const Json::Dict& yellow_pages_json);
    void ParseRubrics(const Json::Dict& yellow_pages_json);
    void ParseCompanyNearbyStops(serialization::Company& comp, const Json::Dict& company_dict);
    void ParseCompanyRubrics(serialization::Company& comp, const Json::Dict& company_dict);
    void ParseCompanyUrls(serialization::Company& comp, const Json::Dict& company_dict);
    void ParseCompanyAddress(serialization::Company& comp, const Json::Dict& company_dict);
    void ParseCompanyNames(serialization::Company& comp, const Json::Dict& company_dict);
    void ParseCompanyPhones(serialization::Company& comp, const Json::Dict& company_dict);
    void ParseCompanyWorkingTime(serialization::Company& comp, const Json::Dict& company_dict);
    void BuildIntervals();
};

