#pragma once

#include "i_update_operation.h"
#include "bus_info.h"
#include "bus_schedule.h"

namespace Operations {
    class AddBus : public IUpdateOperation {
    public:
        AddBus(Models::BusInfo info);

        virtual void Do(Catalog::BusShedule& bus_schedule) override;
    private:
        const Models::BusInfo _info;
    };
}

