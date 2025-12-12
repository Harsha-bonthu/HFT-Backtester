#pragma once
#include <string>
#include <vector>
#include "data_loader.hpp"

namespace hft {

struct Trade {
    std::int64_t entry_ts;
    double entry_price;
    std::int64_t exit_ts;
    double exit_price;
    int quantity; // positive for long, negative for short
};

struct StrategyContext {
    double cash = 100000.0;
    int position = 0;
};

class Strategy {
public:
    virtual ~Strategy() = default;
    virtual std::string name() const = 0;
    virtual void on_bar(const Bar& bar, StrategyContext& ctx, std::vector<Trade>& trades) = 0;
};

}
