#pragma once

#include "test_runner.h"
#include "bus_schedule.h"
#include "text_formatter.h"
#include "text_parser.h"
#include "executor.h"
#include "json_parser.h"
#include "json_formatter.h"
#include "utils.h"
#include "document.h"
#include "circle.h"
#include "text.h"
#include "polyline.h"

#include <sstream>
#include <vector>
#include <string>


namespace UnitTests {
	class TestFixture {
	public:
		static void Run();

	private:
		static void RunTests();
		static void PerformOperationsText();
		static void PerformOperationsJson();
		static void SvgTest();
	};

}

