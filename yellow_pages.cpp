#include "yellow_pages.h"


YellowPages::YellowPages(const Json::Dict& yellow_pages_json)
{
    ParseRubrics(yellow_pages_json);
    ParseCompanies(yellow_pages_json);
}

YellowPages::YellowPages(serialization::Database&& database)
{
    _database = std::move(database);
    BuildIntervals();
}

std::vector<CompanyInfo> YellowPages::FindCompanies(const CompanyQuery& query) const
{
    std::vector<CompanyInfo> result;
    result.reserve(_database.companies_size());
    bool name_match = false;
    bool url_match = false;
    bool rubric_match = false;
    bool phone_match = false;

    for (int index = 0; index < _database.companies_size(); ++index) {
        const auto& source_company = _database.companies(index);

        name_match = query.names.empty();
        if (!name_match && source_company.names_size() > 0) {
            for (const auto& source_name : source_company.names()) {
                if (query.names.find(source_name.value()) != query.names.end()) {
                    name_match = true;
                    break;
                }
            }
        }

        if (!name_match) {
            continue;
        }

        url_match = query.urls.empty();
        if (!url_match && source_company.urls_size() > 0) {
            for (const auto& source_url : source_company.urls()) {
                if (query.urls.find(source_url.value()) != query.urls.end()) {
                    url_match = true;
                    break;
                }
            }
        }

        if (!url_match) {
            continue;
        }

        rubric_match = query.rubrics.empty();
        if (!rubric_match && source_company.rubrics_size() > 0) {
            for (const auto& source_rubric_id : source_company.rubrics()) {
                const auto& source_rubric = _database.rubrics().at(source_rubric_id);
                if (query.rubrics.find(source_rubric.name()) != query.rubrics.end()) {
                    rubric_match = true;
                    break;
                }
            }
        }

        if (!rubric_match) {
            continue;
        }

        phone_match = query.phones.empty();
        if (!phone_match && source_company.phones_size() > 0) {
            for (const auto& source_phone : source_company.phones()) {
                if (phone_match) {
                    break;
                }
                for (const auto& target_phone : query.phones) {
                    phone_match = target_phone.DoesPhoneMatch(source_phone);
                    if (phone_match) {
                        break;
                    }
                }

            }
        }

        if (!phone_match) {
            continue;
        }

        for (const auto& name : source_company.names()) {
            if (name.type() == serialization::Name_Type::Name_Type_MAIN) {
                result.push_back(CompanyInfo{ name.value(), index });
                break;
            }
        }
    }

    return std::move(result);
}

void YellowPages::Serialize(serialization::Database& catalog) const
{
    catalog = std::move(_database);
}

int YellowPages::GetCompaniesCount() const
{
    return _database.companies_size();
}

const serialization::Company& YellowPages::GetCompany(int index) const
{
    return _database.companies(index);
}

std::string YellowPages::GetRubricName(uint64_t rubric_id) const
{
    return _database.rubrics().at(rubric_id).name();
}

double YellowPages::GetWaitTime(int index, double arrival_time) const
{
    const double minutes_per_week = 7.0 * 24.0 * 60.0;

    const auto& it = _working_intervals.find(index);

    if (it == _working_intervals.end()) {
        return -1.0;
    }
    while (arrival_time >= minutes_per_week)
    {
        arrival_time = arrival_time - minutes_per_week;
    }

    const auto& intervals = it->second;

    auto upper_bound = std::upper_bound(intervals.begin(), intervals.end(), arrival_time,
        [](double time, const auto& rhs) {return time < rhs.first; });

    auto prev = upper_bound == intervals.begin()
        ? std::prev(intervals.end())
        : std::prev(upper_bound);

    if (arrival_time >= prev->first && arrival_time < prev->second) {
        return -1.0;
    }

    if (upper_bound == intervals.end()) {
        return minutes_per_week - arrival_time + intervals.begin()->first;
    }

    return upper_bound->first - arrival_time;
}

void YellowPages::ParseCompanies(const Json::Dict& yellow_pages_json)
{
    const auto& company_arr = yellow_pages_json.at("companies").AsArray();
    for (const auto& company : company_arr) {
        serialization::Company* comp = _database.add_companies();;
        const auto& company_dict = company.AsMap();
        ParseCompanyAddress(*comp, company_dict);
        ParseCompanyNames(*comp, company_dict);
        ParseCompanyPhones(*comp, company_dict);
        ParseCompanyUrls(*comp, company_dict);
        ParseCompanyRubrics(*comp, company_dict);
        ParseCompanyWorkingTime(*comp, company_dict);
        ParseCompanyNearbyStops(*comp, company_dict);
    }
}

void YellowPages::ParseRubrics(const Json::Dict& yellow_pages_json)
{
    auto& rubrics = yellow_pages_json.at("rubrics").AsMap();
    for (const auto& rubric : rubrics)
    {
        uint64_t key;
        std::istringstream iss(rubric.first);
        iss >> key;

        serialization::Rubric r;
        const auto& rubric_dict = rubric.second.AsMap();
        r.set_name(rubric_dict.at("name").AsString());
        //if (rubric_dict.find("keywords") != rubric_dict.end()) {
        //    const auto& keywords_arr = rubric_dict.at("keywords").AsArray();
        //    for (const auto& keyword : keywords_arr) {
        //        r.add_keywords(keyword.AsString());
        //    }
        //}
        (*_database.mutable_rubrics())[key] = std::move(r);
    }
}

void YellowPages::ParseCompanyNearbyStops(serialization::Company& comp, const Json::Dict& company_dict)
{
    const auto& nearby_stops = company_dict.find("nearby_stops");
    if (nearby_stops != company_dict.end()) {
        for (const auto& nearby_stop : nearby_stops->second.AsArray()) {
            serialization::NearbyStop* nearby_stop_value = comp.add_nearby_stops();
            const auto& nearby_stop_dict = nearby_stop.AsMap();

            const auto& nearby_stop_name = nearby_stop_dict.find("name");
            if (nearby_stop_name != nearby_stop_dict.end()) {
                nearby_stop_value->set_name(nearby_stop_name->second.AsString());
            }

            const auto& nearby_stop_meters = nearby_stop_dict.find("meters");
            if (nearby_stop_meters != nearby_stop_dict.end()) {
                nearby_stop_value->set_meters(nearby_stop_meters->second.AsInt());
            }
        }
    }
}

void YellowPages::ParseCompanyRubrics(serialization::Company& comp, const Json::Dict& company_dict)
{
    const auto& comp_rubrics = company_dict.find("rubrics");
    if (comp_rubrics != company_dict.end()) {
        for (const auto& rubric_node : comp_rubrics->second.AsArray()) {
            comp.add_rubrics(rubric_node.AsInt());
        }
    }
}

void YellowPages::ParseCompanyUrls(serialization::Company& comp, const Json::Dict& company_dict)
{
    const auto& urls = company_dict.find("urls");
    if (urls != company_dict.end()) {
        const auto& urls_arr = urls->second.AsArray();
        for (const auto& url : urls_arr) {
            serialization::Url* url_value = comp.add_urls();
            url_value->set_value(url.AsMap().at("value").AsString());
        }
    }
}

void YellowPages::ParseCompanyAddress(serialization::Company& comp, const Json::Dict& company_dict)
{
    serialization::Address* address = comp.mutable_address();
    const auto& address_node = company_dict.find("address");
    if (address_node != company_dict.end()) {
        const auto& address_dict = company_dict.at("address").AsMap();

        const auto& formatted = address_dict.find("formatted");
        if (formatted != address_dict.end()) {
            address->set_formatted(formatted->second.AsString());
        }

        const auto& components = address_dict.find("components");
        if (components != address_dict.end()) {
            const auto& components_arr = components->second.AsArray();
            for (const auto& component : components_arr) {
                serialization::AddressComponent* component_value = address->add_components();
                const auto& component_dict = component.AsMap();
                component_value->set_value(component_dict.at("value").AsString());
            }
        }

        const auto& coords = address_dict.find("coords");
        if (coords != address_dict.end()) {
            const auto& coords_dict = coords->second.AsMap();
            std::stringstream lat_str(coords_dict.at("lat").AsString());
            std::stringstream lon_str(coords_dict.at("lon").AsString());
            double lat;
            double lon;
            lat_str >> lat;
            lon_str >> lon;
            address->mutable_coords()->set_lat(lat);
            address->mutable_coords()->set_lon(lon);
        }

        const auto& comment = address_dict.find("comment");
        if (comment != address_dict.end()) {
            address->set_comment(comment->second.AsString());
        }
    }
}

void YellowPages::ParseCompanyNames(serialization::Company& comp, const Json::Dict& company_dict)
{
    const auto& names_arr = company_dict.at("names").AsArray();
    for (const auto& name : names_arr) {
        const auto& name_dict = name.AsMap();
        serialization::Name* name_value = comp.add_names();;
        name_value->set_value(name_dict.at("value").AsString());
        const auto& type_node = name_dict.find("type");
        if (type_node == name_dict.end()) {
            name_value->set_type(serialization::Name_Type::Name_Type_MAIN);
        }
        else {
            if (type_node->second.AsString() == "MAIN") {
                name_value->set_type(serialization::Name_Type::Name_Type_MAIN);
            }
            else if (type_node->second.AsString() == "SHORT") {
                name_value->set_type(serialization::Name_Type::Name_Type_SHORT);
            }
            else {
                name_value->set_type(serialization::Name_Type::Name_Type_SYNONYM);
            }
        }

    }
}

void YellowPages::ParseCompanyPhones(serialization::Company& comp, const Json::Dict& company_dict)
{
    const auto& phones = company_dict.find("phones");
    if (phones != company_dict.end()) {
        for (const auto& phone_node : phones->second.AsArray()) {
            serialization::Phone* phone = comp.add_phones();
            const auto& phone_dict = phone_node.AsMap();

            const auto& formatted = phone_dict.find("formatted");
            if (formatted != phone_dict.end()) {
                phone->set_formatted(formatted->second.AsString());
            }

            const auto& type = phone_dict.find("type");
            if (type != phone_dict.end()) {
                if (type->second.AsString() == "PHONE") {
                    phone->set_type(serialization::Phone_Type::Phone_Type_PHONE);
                }
                else if (type->second.AsString() == "FAX") {
                    phone->set_type(serialization::Phone_Type::Phone_Type_FAX);
                }
            }
            else {
                phone->set_type(serialization::Phone_Type::Phone_Type_PHONE);
            }

            const auto& country_code = phone_dict.find("country_code");
            if (country_code != phone_dict.end()) {
                phone->set_country_code(country_code->second.AsString());
            }

            const auto& local_code = phone_dict.find("local_code");
            if (local_code != phone_dict.end()) {
                phone->set_local_code(local_code->second.AsString());
            }

            const auto& number = phone_dict.find("number");
            if (number != phone_dict.end()) {
                phone->set_number(number->second.AsString());
            }

            const auto& extension = phone_dict.find("extension");
            if (extension != phone_dict.end()) {
                phone->set_extension(extension->second.AsString());
            }

            const auto& description = phone_dict.find("description");
            if (description != phone_dict.end()) {
                phone->set_description(description->second.AsString());
            }
        }


    }
}

void YellowPages::ParseCompanyWorkingTime(serialization::Company& comp, const Json::Dict& company_dict)
{
    auto it = company_dict.find("working_time");
    if (it != company_dict.end()) {
        const auto& working_time = it->second.AsMap();
        auto intervals_it = working_time.find("intervals");
        if (intervals_it != working_time.end()) {
            const auto& intervals = intervals_it->second.AsArray();
            for (const auto& interval : intervals) {
                const auto& interval_dict = interval.AsMap();
                auto* interval_value = comp.mutable_working_time()->add_intervals();
                
                const auto& day = interval_dict.at("day").AsString();
                if (day == "MONDAY") {
                    interval_value->set_day(serialization::WorkingTimeInterval_Day_MONDAY);
                }
                else if (day == "TUESDAY") {
                    interval_value->set_day(serialization::WorkingTimeInterval_Day_TUESDAY);
                }
                else if (day == "WEDNESDAY") {
                    interval_value->set_day(serialization::WorkingTimeInterval_Day_WEDNESDAY);
                }
                else if (day == "THURSDAY") {
                    interval_value->set_day(serialization::WorkingTimeInterval_Day_THURSDAY);
                }
                else if (day == "FRIDAY") {
                    interval_value->set_day(serialization::WorkingTimeInterval_Day_FRIDAY);
                }
                else if (day == "SATURDAY") {
                    interval_value->set_day(serialization::WorkingTimeInterval_Day_SATURDAY);
                }
                else if (day == "SUNDAY") {
                    interval_value->set_day(serialization::WorkingTimeInterval_Day_SUNDAY);
                }
                else if (day == "EVERYDAY") {
                    interval_value->set_day(serialization::WorkingTimeInterval_Day_EVERYDAY);
                }

                interval_value->set_minutes_from(interval_dict.at("minutes_from").AsInt());
                interval_value->set_minutes_to(interval_dict.at("minutes_to").AsInt());
            }
        }
    }
}

void YellowPages::BuildIntervals()
{
    _day_to_index[serialization::WorkingTimeInterval_Day::WorkingTimeInterval_Day_MONDAY] = 0;
    _day_to_index[serialization::WorkingTimeInterval_Day::WorkingTimeInterval_Day_TUESDAY] = 1;
    _day_to_index[serialization::WorkingTimeInterval_Day::WorkingTimeInterval_Day_WEDNESDAY] = 2;
    _day_to_index[serialization::WorkingTimeInterval_Day::WorkingTimeInterval_Day_THURSDAY] = 3;
    _day_to_index[serialization::WorkingTimeInterval_Day::WorkingTimeInterval_Day_FRIDAY] = 4;
    _day_to_index[serialization::WorkingTimeInterval_Day::WorkingTimeInterval_Day_SATURDAY] = 5;
    _day_to_index[serialization::WorkingTimeInterval_Day::WorkingTimeInterval_Day_SUNDAY] = 6;

    const double minutes_per_day = 24.0 * 60.0;

    for (int i = 0; i < _database.companies_size(); ++i) {
        const auto& company = _database.companies(i);

        if (company.working_time().intervals_size() == 0) {
            continue;
        }

        std::vector<std::pair<double, double>> intervals;
        for (const auto& interval : company.working_time().intervals()) {
            if (interval.day() == serialization::WorkingTimeInterval_Day::WorkingTimeInterval_Day_EVERYDAY) {
                for (size_t i = 0; i < 7; ++i) {
                    double start = i * minutes_per_day + interval.minutes_from();
                    double end = i * minutes_per_day + interval.minutes_to();
                    intervals.push_back({ start, end });
                }
            }
            else {
                int32_t day_of_week = _day_to_index.at(interval.day());
                double start = minutes_per_day * day_of_week + interval.minutes_from();
                double end = minutes_per_day * day_of_week + interval.minutes_to();
                intervals.push_back({ start, end });
            }
        }
        std::sort(intervals.begin(), intervals.end(), [](const auto& lhs, const auto& rhs) {return lhs.first < rhs.first; });
        _working_intervals[i] = std::move(intervals);
    }
}

CompanyQuery CompanyQuery::Create(const Json::Dict& attrs)
{
    CompanyQuery result;
    auto names = attrs.find("names");
    if (names != attrs.end()) {
        for (const auto& name : names->second.AsArray()) {
            result.names.insert(name.AsString());
        }
    }

    auto urls = attrs.find("urls");
    if (urls != attrs.end()) {
        for (const auto& url : urls->second.AsArray()) {
            result.urls.insert(url.AsString());
        }
    }

    auto rubrics = attrs.find("rubrics");
    if (rubrics != attrs.end()) {
        for (const auto& rubric : rubrics->second.AsArray()) {
            result.rubrics.insert(rubric.AsString());
        }
    }

    auto phones = attrs.find("phones");
    if (phones != attrs.end()) {
        for (const auto& phoneNode : phones->second.AsArray()) {
            const auto& phone = phoneNode.AsMap();
            PhoneQuery templ;
            auto type = phone.find("type");
            if (type != phone.end()) {
                templ.type = type->second.AsString();
            }
            auto country_code = phone.find("country_code");
            if (country_code != phone.end()) {
                templ.country_code = country_code->second.AsString();
            }
            auto local_code = phone.find("local_code");
            if (local_code != phone.end()) {
                templ.local_code = local_code->second.AsString();
            }
            auto number = phone.find("number");
            if (number != phone.end()) {
                templ.number = number->second.AsString();
            }
            auto extension = phone.find("extension");
            if (extension != phone.end()) {
                templ.extension = extension->second.AsString();
            }
            result.phones.push_back(std::move(templ));
        }
    }
    return std::move(result);
}

bool PhoneQuery::DoesPhoneMatch(const serialization::Phone& object) const
{
    if (!extension.empty() && extension != object.extension()) {
        return false;
    }
    if (!type.empty() && phone_type() != object.type()) {
        return false;
    }
    if (!country_code.empty() && country_code != object.country_code()) {
        return false;
    }
    if (
        (!local_code.empty() || !country_code.empty())
        && local_code != object.local_code()
        ) {
        return false;
    }
    return number == object.number();
}

serialization::Phone_Type PhoneQuery::phone_type() const
{
    if (type == "PHONE")
        return serialization::Phone_Type::Phone_Type_PHONE;
    return serialization::Phone_Type::Phone_Type_FAX;
}

std::string GetCompanyName(const serialization::Company& company)
{
    for (const auto& name : company.names()) {
        if (name.type() == serialization::Name_Type::Name_Type_MAIN) {
            return name.value();
        }
    }
	return std::string();
}
