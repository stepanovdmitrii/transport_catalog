#pragma once

#include "i_read_operation.h"
#include "bus_schedule.h"
#include "i_formatter.h"

#include <string>

namespace Operations {
    class FindWay : public IReadOperation {
    public:
        FindWay(std::string from, std::string to, size_t request_id);

        // Унаследовано через IReadOperation
        virtual void Do(Catalog::BusShedule& bus_schedule, IO::IFormatter& formatter) override;
    private:
        std::string _from;
        std::string _to;
        size_t _request_id;
    };
}

