#pragma once
#include "strategy.hpp"
#include <deque>

namespace hft {

class MomentumStrategy : public Strategy {
public:
    explicit MomentumStrategy(int lookback = 20, int qty = 1)
        : lookback_(lookback), qty_(qty) {}
    std::string name() const override { return "Momentum"; }
    void on_bar(const Bar& bar, StrategyContext& ctx, std::vector<Trade>& trades) override {
        prices_.push_back(bar.close);
        if ((int)prices_.size() > lookback_) prices_.pop_front();
        if ((int)prices_.size() < lookback_) return;
        double ret = (prices_.back() - prices_.front()) / prices_.front();
        if (ret > 0.0 && ctx.position <= 0) {
            // go long
            int buy_qty = qty_ - std::max(0, ctx.position);
            if (buy_qty > 0) {
                ctx.cash -= buy_qty * bar.close;
                ctx.position += buy_qty;
                trades.push_back({bar.ts, bar.close, bar.ts, bar.close, buy_qty});
            }
        } else if (ret < 0.0 && ctx.position >= 0) {
            // go short
            int sell_qty = qty_ + std::min(0, ctx.position);
            if (sell_qty > 0) {
                ctx.cash += sell_qty * bar.close;
                ctx.position -= sell_qty;
                trades.push_back({bar.ts, bar.close, bar.ts, bar.close, -sell_qty});
            }
        }
    }
private:
    int lookback_;
    int qty_;
    std::deque<double> prices_;
};

}
