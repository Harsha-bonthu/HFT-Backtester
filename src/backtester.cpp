#include "backtester.hpp"
#include <algorithm>

namespace hft {

BacktestResult Backtester::run(const std::vector<Bar>& bars, Strategy& strat, const CostModel& costs, int lot) {
    BacktestResult res{};
    StrategyContext ctx{};
    double equity = ctx.cash;
    std::vector<double> returns;
    res.equity_curve.reserve(bars.size());

    for (size_t i = 0; i < bars.size(); ++i) {
        const auto& b = bars[i];
        std::vector<Trade> new_trades;
        strat.on_bar(b, ctx, new_trades);
        for (auto& t : new_trades) {
            // apply transaction costs at trade time
            double c = costs.cost(t.entry_price, t.quantity);
            ctx.cash -= c;
            res.trades.push_back(t);
        }
        // Mark-to-market equity update
        equity = ctx.cash + ctx.position * b.close;
        res.equity_curve.push_back(equity);
        if (i > 0) {
            double ret = (res.equity_curve[i] - res.equity_curve[i-1]) / res.equity_curve[i-1];
            returns.push_back(ret);
        }
    }

    res.sharpe = sharpe_ratio(returns);
    res.drawdown = max_drawdown(res.equity_curve);
    res.final_equity = res.equity_curve.empty() ? ctx.cash : res.equity_curve.back();
    return res;
}

}
