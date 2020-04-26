#pragma once

#include "i_read_operation.h"
#include "i_formatter.h"
#include "bus_schedule.h"
#include "stop_stat_info.h"

#include <string>

namespace Operations {
    class StopStat : public IReadOperation {
    public:
        StopStat(std::string stop, size_t request_id = 0);

        // Унаследовано через IReadOperation
        virtual void Do(Catalog::BusShedule& bus_schedule, IO::IFormatter& formatter) override;
    private:
        std::string _stop;
        size_t _request_id;
        Models::StopStatInfo Collect(Catalog::BusShedule& bus_schedule);
    };
}

