#include "advanced_backtester.hpp"
#include "performance.hpp"
#include <algorithm>
#include <cmath>

namespace hft {

AssetBacktest AdvancedBacktester::run(const std::string& asset_name,
                                      const std::vector<Bar>& bars,
                                      Strategy& strat,
                                      const CostModel& costs,
                                      const RiskControl& risk,
                                      const OrderBook& lob) {
    AssetBacktest res;
    res.asset = asset_name;
    
    StrategyContext ctx{};
    ctx.cash = 100000.0;
    
    std::vector<double> prices;
    res.equity_curve.reserve(bars.size());
    res.pnl_series.reserve(bars.size());
    
    double daily_pnl = 0;
    
    for (size_t i = 0; i < bars.size(); ++i) {
        const auto& b = bars[i];
        prices.push_back(b.close);
        
        // Compute volatility for position scaling
        double vol = compute_volatility(prices, 20);
        
        // Execute strategy
        std::vector<Trade> new_trades;
        strat.on_bar(b, ctx, new_trades);
        
        for (auto& t : new_trades) {
            // Check risk controls: position limits
            if (std::abs(ctx.position + t.quantity) > risk.max_position) {
                continue; // skip trade if violates max position
            }
            
            // Fill at LOB price with impact
            bool is_buy = t.quantity > 0;
            double fill_price = lob.get_fill_price(b.close, t.quantity, is_buy);
            
            // Apply transaction costs
            double cost = costs.cost(fill_price, t.quantity);
            ctx.cash -= t.quantity * fill_price + cost;
            ctx.position += t.quantity;
            
            t.entry_price = fill_price; // record actual fill
            res.trades.push_back(t);
        }
        
        // Mark-to-market and apply stop-loss / take-profit
        double mtm_equity = ctx.cash + ctx.position * b.close;
        double unrealized_pnl = ctx.position * (b.close - (prices.size() > 1 ? prices[prices.size()-2] : b.close));
        daily_pnl += unrealized_pnl;
        
        // Check daily loss limit
        if (daily_pnl < -risk.max_daily_loss) {
            ctx.position = 0; // liquidate on max daily loss
            ctx.cash = mtm_equity;
            daily_pnl = 0;
        }
        
        res.equity_curve.push_back(mtm_equity);
        res.pnl_series.push_back(unrealized_pnl);
    }
    
    res.num_trades = res.trades.size();
    res.final_equity = res.equity_curve.empty() ? ctx.cash : res.equity_curve.back();
    
    if (!res.equity_curve.empty()) {
        std::vector<double> returns;
        for (size_t i = 1; i < res.equity_curve.size(); ++i) {
            double ret = (res.equity_curve[i] - res.equity_curve[i-1]) / res.equity_curve[i-1];
            returns.push_back(ret);
        }
        if (!returns.empty()) {
            double mean = 0; for (double r : returns) mean += r; mean /= returns.size();
            double var = 0; for (double r : returns) { double d = r - mean; var += d*d; } var /= returns.size();
            double sd = std::sqrt(var);
            res.sharpe = (sd > 0) ? (mean / sd * std::sqrt(252.0)) : 0.0;
        }
        
        double peak = res.equity_curve[0];
        for (double v : res.equity_curve) {
            if (v > peak) peak = v;
            double dd = (peak - v) / peak;
            if (dd > res.max_dd) res.max_dd = dd;
        }
    }
    
    return res;
}

}
