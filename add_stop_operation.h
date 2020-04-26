#pragma once

#include "i_update_operation.h"
#include "stop_info.h"
#include "bus_schedule.h"

namespace Operations {
    class AddStop : public IUpdateOperation {
    public:
        AddStop(Models::StopInfo info);

        virtual void Do(Catalog::BusShedule& bus_schedule) override;
    private:
        const Models::StopInfo _info;
    };
}

