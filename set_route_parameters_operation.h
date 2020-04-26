#pragma once

#include "i_update_operation.h"
#include "bus_schedule.h"

namespace Operations {
    class SetRouteParameters : public IUpdateOperation {
    public:
        SetRouteParameters(double wait_time, double velocity);

        virtual void Do(Catalog::BusShedule& bus_schedule) override;
    private:
        double _wait_time;
        double _velocity;
    };
}

