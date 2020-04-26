#include "json_formatter.h"

IO::JsonFormatter::JsonFormatter(std::ostream& output) : _output(output)
{
}

void IO::JsonFormatter::Write(const Models::BusStatInfo& info)
{
	_bus_infos.push_back(info);
}

void IO::JsonFormatter::Write(const Models::StopStatInfo& info)
{
	_stop_infos.push_back(info);
}

void IO::JsonFormatter::Write(const Models::RouteInfo& info)
{
	_route_infos.push_back(info);
}

void IO::JsonFormatter::Flush()
{
	WriteSquareBrace(true);
	WriteElements();
	WriteSquareBrace(false);
}

void IO::JsonFormatter::WriteSquareBrace(bool open)
{
	if (open) {
		_output << "[";
	}
	else {
		_output << "]";
	}
}

void IO::JsonFormatter::WriteBrace(bool open)
{
	if (open) {
		_output << "{";
	}
	else {
		_output << "}";
	}
}

void IO::JsonFormatter::WriteBusElement(const Models::BusStatInfo& bus)
{
	WriteBrace(true);
	if (bus.found) {
		_output << "\"route_length\":" << bus.length << ",\"request_id\":" << bus.request_id << ",\"curvature\":" << bus.curvature << ",\"stop_count\":" << bus.total_stops << ",\"unique_stop_count\":" << bus.unique_stops;
	}
	else {
		_output << "\"request_id\":" << bus.request_id << ",\"error_message\":\"not found\"";
	}
	WriteBrace(false);
}

void IO::JsonFormatter::WriteStopElement(const Models::StopStatInfo& stop)
{
	WriteBrace(true);
	if (stop.found) {
		_output << "\"buses\":";
		WriteSquareBrace(true);
		bool first = true;
		for (const std::string& b : stop.buses) {
			if (!first) {
				_output << ",";
			}
			_output << "\"" << b << "\"";
			first = false;
		}

		WriteSquareBrace(false);

		_output << ",\"request_id\":" << stop.request_id;
	}
	else {
		_output << "\"request_id\":" << stop.request_id << ",\"error_message\":\"not found\"";
	}
	WriteBrace(false);
}

void IO::JsonFormatter::WriteRouteElement(const Models::RouteInfo& route)
{
	WriteBrace(true);
	if (route.found) {
		_output << "\"request_id\":" << route.request_id << ",\"total_time\":" << route.total_time << ",\"items\":";
		WriteSquareBrace(true);
		bool first = true;
		for (const auto& s : route.segments) {
			if (!first) {
				_output << ",";
			}
			WriteBrace(true);
			_output << "\"type\":\"Wait\",\"stop_name\":\"" << s.stop_from << "\",\"time\":" << s.wait_time;
			WriteBrace(false);
			_output << ",";
			WriteBrace(true);
			_output << "\"type\":\"Bus\",\"bus\":\"" << s.bus << "\",\"span_count\":" << s.span_count << ",\"time\":" << s.trip_time;
			WriteBrace(false);
			first = false;
		}
		WriteSquareBrace(false);
	}
	else {
		_output << "\"request_id\":" << route.request_id << ",\"error_message\":\"not found\"";
	}
	WriteBrace(false);
}

void IO::JsonFormatter::WriteMapElement(const Models::DrawMapInfo& map)
{
	std::stringstream ss;
	map.svg->Render(ss);
	

	WriteBrace(true);
	_output << "\"request_id\":" << map.request_id << ",\"map\":\"";
	for (const char& ch : ss.str()) {
		if (ch == '"') {
			_output << "\\\"";
		}
		else if (ch == '\\') {
			_output << "\\\\";
		}
		else
		{
			_output << ch;
		}
	}
	_output << "\"";
	WriteBrace(false);
}

void IO::JsonFormatter::WriteElements()
{
	bool first = true;
	for (const auto& b : _bus_infos) {
		if (!first) {
			_output << ",";
		}
		WriteBusElement(b);
		first = false;
	}
	for (const auto& s : _stop_infos) {
		if (!first) {
			_output << ",";
		}
		WriteStopElement(s);
		first = false;
	}
	for (const auto& r : _route_infos) {
		if (!first) {
			_output << ",";
		}
		WriteRouteElement(r);
		first = false;
	}
	for (const auto& m : _map_infos) {
		if (!first) {
			_output << ",";
		}
		WriteMapElement(m);
		first = false;
	}
}

void IO::JsonFormatter::Write(const Models::DrawMapInfo& info)
{
	_map_infos.push_back(info);
}
