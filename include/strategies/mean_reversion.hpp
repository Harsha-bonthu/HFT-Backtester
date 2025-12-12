#pragma once
#include "strategy.hpp"
#include <deque>

namespace hft {

class MeanReversionStrategy : public Strategy {
public:
    explicit MeanReversionStrategy(int lookback = 20, double threshold = 0.01, int qty = 1)
        : lookback_(lookback), threshold_(threshold), qty_(qty) {}
    std::string name() const override { return "MeanReversion"; }
    void on_bar(const Bar& bar, StrategyContext& ctx, std::vector<Trade>& trades) override {
        prices_.push_back(bar.close);
        if ((int)prices_.size() > lookback_) prices_.pop_front();
        if ((int)prices_.size() < lookback_) return;
        double sum = 0.0; for (double p : prices_) sum += p; double avg = sum / prices_.size();
        double dev = (bar.close - avg) / avg;
        if (dev > threshold_ && ctx.position > -qty_) {
            // overpriced -> short
            int to_sell = std::min(qty_, ctx.position + qty_);
            if (to_sell > 0) {
                ctx.cash += to_sell * bar.close;
                ctx.position -= to_sell;
                trades.push_back({bar.ts, bar.close, bar.ts, bar.close, -to_sell});
            }
        } else if (dev < -threshold_ && ctx.position < qty_) {
            // underpriced -> buy
            int to_buy = std::min(qty_, qty_ - ctx.position);
            if (to_buy > 0) {
                ctx.cash -= to_buy * bar.close;
                ctx.position += to_buy;
                trades.push_back({bar.ts, bar.close, bar.ts, bar.close, to_buy});
            }
        }
    }
private:
    int lookback_;
    double threshold_;
    int qty_;
    std::deque<double> prices_;
};

}
