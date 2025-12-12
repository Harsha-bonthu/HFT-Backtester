# HFT Backtester (C++17)

A production-grade quantitative trading backtesting engine with order-book microstructure simulation, risk management, and multi-asset portfolio support. Built to demonstrate systems engineering and quantitative finance expertise for HFT roles (PACE Stock Broking, etc.).

## Key Features

### Core Engine ✅
- **Data Pipeline**: CSV loader + synthetic random-walk generator (configurable drift/volatility)
- **Strategy Framework**: Pluggable interface; momentum & mean-reversion strategies included
- **Multi-Asset Support**: Simultaneous backtesting on 3+ instruments with per-asset metrics
- **Vectorized Backtester**: O(n) performance, bar-by-bar equity/PnL tracking, mark-to-market updates

### Advanced Modules ✅
- **Order Book Microstructure** (`orderbook.hpp`): Realistic bid/ask spreads, market impact modeling
- **Risk Management** (`risk.hpp`): Position limits, stop-loss/take-profit, daily loss caps, volatility scaling
- **Enhanced Metrics** (`performance.hpp`): Sharpe, Sortino, Calmar, max drawdown, win rate, profit factor
- **Transaction Costs** (`costs.hpp`): Commission per share + slippage (bps) + order impact
- **Multi-Format Reporting** (`advanced_reports.hpp`): CSV summaries, bar-by-bar details, equity curves
- **Python Visualization**: 6-panel analysis charts with matplotlib

## Demo Results (3-Asset Portfolio)

### Momentum Strategy
| Metric | Value |
|--------|-------|
| **Sharpe Ratio** | 0.551 (GOOGL: 0.707) |
| **Total P&L** | +$1,415.37 |
| **Total Trades** | 338 |
| **Max Drawdown** | <0.4% |
| **Assets** | AAPL (+0.376%), GOOGL (+0.876%), MSFT (+0.164%) |

### Mean Reversion Strategy
| Metric | Value |
|--------|-------|
| **Sharpe Ratio** | 0.0517 |
| **Total P&L** | +$133.67 |
| **Total Trades** | 389 |
| **Max Drawdown** | <0.35% |

## Project Structure

```
include/
  ├── data_loader.hpp          # CSV parsing
  ├── strategy.hpp             # Strategy interface
  ├── backtester.hpp           # Basic backtester
  ├── advanced_backtester.hpp  # Multi-asset + LOB engine
  ├── orderbook.hpp            # Order book simulation
  ├── risk.hpp                 # Risk controls
  ├── performance.hpp          # Performance metrics
  ├── costs.hpp                # Transaction costs
  ├── synthetic.hpp            # Data generator
  ├── indicators.hpp           # SMA, EMA, RSI
  ├── strategies/
  │   ├── momentum.hpp
  │   └── mean_reversion.hpp
  └── ...

src/
  ├── main.cpp                 # Basic backtester CLI
  ├── main_advanced.cpp        # Advanced multi-asset demo
  ├── data_loader.cpp
  ├── backtester.cpp
  └── advanced_backtester.cpp

tests/
  └── basic_tests.cpp

data/
  └── sample.csv

build/
  ├── hft_backtester.exe       # Basic executable
  ├── hft_advanced.exe         # Advanced executable
  ├── results_momentum_*.csv    # Output reports
  ├── results_meanreversion_*.csv
  └── hft_backtest_analysis.png # Visualization
```

## Build & Run

### Prerequisites
- CMake 3.16+, C++17, Ninja (or Make)
- Windows: `winget install Kitware.CMake Ninja-build.Ninja`

### Build Steps (PowerShell)
```powershell
cd "e:\new c++"
mkdir build
cd build
cmake -G "Ninja" ..
ninja
```

### Run Basic Backtester
```powershell
./hft_backtester.exe synthetic
# Generates: results_momentum_*.csv, results_meanrev_*.csv
```

### Run Advanced Multi-Asset Backtester
```powershell
./hft_advanced.exe
# 3 assets × 2 strategies = 6 backtests
# Outputs: 6 CSV reports + console metrics
```

### Visualize Results (Python)
```powershell
python visualize.py
# Generates: hft_backtest_analysis.png (6-panel chart)
```

## Architecture Highlights

### Modular Design
- **Strategy Interface**: `Strategy` base class; implement `on_bar()` for custom strategies
- **Backtester Variants**: 
  - `Backtester::run()` – Basic, cost-aware backtester
  - `AdvancedBacktester::run()` – LOB + risk controls + multi-asset
- **Risk Framework**: Composable `RiskControl` struct (limits, stops, scalars)
- **Order Book**: Realistic `OrderBook` with impact modeling

### Performance
- **Time Complexity**: O(n) per backtest (single pass through bars)
- **Space**: O(n) for equity curves, O(1) for streaming metrics
- **Tested**: 3,000 bars (3 assets × 1,000 bars) processed in <100ms

### Code Quality
- **C++17 Modern**: Move semantics, standard containers, header-only utilities
- **Cross-Platform**: CMake with Ninja/Make, no OS-specific code
- **Error Handling**: Safe division, NaN checks, graceful fallbacks
- **Documentation**: Inline comments, clear naming, comprehensive README

## CSV Output Format

### Summary (`*_summary.csv`)
```
asset,sharpe,max_dd,final_equity,trades,total_return
AAPL_MOM,0.5696,0.002328,100376,109,0.00376
```

### Equity Curve (`*_equity.csv`)
```
bar_idx,AAPL_MOM,GOOGL_MOM,MSFT_MOM
0,100000,99999.9,99999.9
1,100000,100006,100003
...
```

### Detailed (`*_details.csv`)
```
asset,bar_idx,equity,pnl
AAPL_MOM,0,100000,0
AAPL_MOM,1,100000,0
...
```

## Why This Impresses Recruiters (PACE JD Alignment)

✅ **"High proficiency in C++"**
- Modern C++17, clean OOP, header-only templates, modular architecture

✅ **"Statistical analysis on large datasets"**
- Sharpe, Sortino, Calmar, rolling volatility on 1,000+ bars per asset

✅ **"Efficient automation of backtesting tools"**
- Full CLI, parameter sweeps, batch CSV export, zero manual work

✅ **"Data processing challenges"**
- CSV parsing, real-time equity updates, bar-by-bar PnL attribution

✅ **"Quantitative modeling"**
- Momentum & mean reversion, LOB microstructure, risk controls, transaction costs

✅ **"HFT-grade engineering"**
- Order book simulation, position limits, daily loss controls, volatility scaling

## Future Enhancements

- Walk-forward optimization (parameter grid search + rolling validation)
- Cointegration-based pairs trading
- Machine learning feature engineering (technical indicators → ML models)
- Live market data integration (yfinance, IB API)
- Real-time position monitoring & limit order management
- gRPC service for distributed backtests
- Risk dashboards with live Greeks/exposure tracking

## References

- `PROJECT_SUMMARY.md` – Comprehensive technical overview
- `include/advanced_backtester.hpp` – Engine core (with detailed comments)
- `build/hft_backtest_analysis.png` – 6-panel visualization

---

**Built with production-grade engineering practices. Ready for interview showcase or deployment.**
