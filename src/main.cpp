#include <iostream>
#include <string>
#include "data_loader.hpp"
#include "backtester.hpp"
#include "strategies/momentum.hpp"
#include "strategies/mean_reversion.hpp"
#include "costs.hpp"
#include "reports.hpp"
#include "synthetic.hpp"

int main(int argc, char** argv) {
    using namespace hft;
    std::vector<Bar> bars;
    if (argc > 1 && std::string(argv[1]) == std::string("synthetic")) {
        bars = hft::generate_random_walk(1000);
        std::cout << "Loaded synthetic dataset (random walk) with " << bars.size() << " bars.\n";
    } else {
        std::string csv = argc > 1 ? argv[1] : "data/sample.csv";
        bars = DataLoader::load_csv(csv);
        if (bars.empty()) {
            bars = hft::generate_random_walk(1000);
            std::cout << "Loaded synthetic dataset (fallback) with " << bars.size() << " bars.\n";
        }
    }

    CostModel costs{0.0, 1.0}; // lower costs for demo visibility

    // Parameter sweep for momentum lookback
    double best_sharpe = -1e9; int best_lb = 0; BacktestResult best_res{};
    for (int lb = 5; lb <= 50; lb += 5) {
        MomentumStrategy mom(lb, 1);
        auto r = Backtester::run(bars, mom, costs, 1);
        if (r.sharpe > best_sharpe) { best_sharpe = r.sharpe; best_lb = lb; best_res = r; }
    }
    std::cout << "Best Momentum LB=" << best_lb << " Sharpe=" << best_sharpe << " FinalEquity=" << best_res.final_equity << "\n";
    write_summary_csv("results_momentum_summary.csv", "Momentum_lb_" + std::to_string(best_lb), best_res);
    write_equity_csv("results_momentum_equity.csv", best_res.equity_curve);

    // Mean Reversion fixed params
    MeanReversionStrategy mr(20, 0.003, 1);
    auto res_mr = Backtester::run(bars, mr, costs, 1);
    std::cout << "MeanReversion Sharpe=" << res_mr.sharpe << " FinalEquity=" << res_mr.final_equity << " MaxDD=" << res_mr.drawdown << "\n";
    write_summary_csv("results_meanrev_summary.csv", "MeanReversion", res_mr);
    write_equity_csv("results_meanrev_equity.csv", res_mr.equity_curve);

    return 0;
}
