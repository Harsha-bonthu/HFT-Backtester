#!/usr/bin/env python3
"""
HFT Backtester Visualization & Analysis
Plots equity curves, PnL, and performance metrics
"""
import pandas as pd
import matplotlib.pyplot as plt
import sys
from pathlib import Path

def plot_results(momentum_equity, mr_equity, momentum_summary, mr_summary):
    """Generate comprehensive plots"""
    fig = plt.figure(figsize=(16, 12))
    
    # Momentum equity curves
    ax1 = plt.subplot(3, 2, 1)
    for col in momentum_equity.columns[1:]:
        ax1.plot(momentum_equity['bar_idx'], momentum_equity[col], label=col, linewidth=1.5)
    ax1.set_title('Momentum Strategy: Equity Curves', fontsize=12, fontweight='bold')
    ax1.set_xlabel('Bar Index')
    ax1.set_ylabel('Equity ($)')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # MR equity curves
    ax2 = plt.subplot(3, 2, 2)
    for col in mr_equity.columns[1:]:
        ax2.plot(mr_equity['bar_idx'], mr_equity[col], label=col, linewidth=1.5)
    ax2.set_title('Mean Reversion Strategy: Equity Curves', fontsize=12, fontweight='bold')
    ax2.set_xlabel('Bar Index')
    ax2.set_ylabel('Equity ($)')
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    # Sharpe ratio comparison
    ax3 = plt.subplot(3, 2, 3)
    assets = list(range(len(momentum_summary)))
    width = 0.35
    ax3.bar([i - width/2 for i in assets], momentum_summary['sharpe'], width, label='Momentum', alpha=0.8)
    ax3.bar([i + width/2 for i in assets], mr_summary['sharpe'], width, label='Mean Reversion', alpha=0.8)
    ax3.set_title('Sharpe Ratio by Asset', fontsize=12, fontweight='bold')
    ax3.set_ylabel('Sharpe Ratio')
    ax3.set_xticks(assets)
    ax3.set_xticklabels(momentum_summary['asset'])
    ax3.legend()
    ax3.grid(True, alpha=0.3, axis='y')
    ax3.axhline(y=0, color='r', linestyle='--', linewidth=1)
    
    # Max Drawdown comparison
    ax4 = plt.subplot(3, 2, 4)
    ax4.bar([i - width/2 for i in assets], momentum_summary['max_dd'], width, label='Momentum', alpha=0.8)
    ax4.bar([i + width/2 for i in assets], mr_summary['max_dd'], width, label='Mean Reversion', alpha=0.8)
    ax4.set_title('Maximum Drawdown by Asset', fontsize=12, fontweight='bold')
    ax4.set_ylabel('Max DD')
    ax4.set_xticks(assets)
    ax4.set_xticklabels(momentum_summary['asset'])
    ax4.legend()
    ax4.grid(True, alpha=0.3, axis='y')
    
    # Total Return
    ax5 = plt.subplot(3, 2, 5)
    ax5.bar([i - width/2 for i in assets], momentum_summary['total_return'] * 100, width, label='Momentum', alpha=0.8)
    ax5.bar([i + width/2 for i in assets], mr_summary['total_return'] * 100, width, label='Mean Reversion', alpha=0.8)
    ax5.set_title('Total Return by Asset (%)', fontsize=12, fontweight='bold')
    ax5.set_ylabel('Return (%)')
    ax5.set_xticks(assets)
    ax5.set_xticklabels(momentum_summary['asset'])
    ax5.legend()
    ax5.grid(True, alpha=0.3, axis='y')
    ax5.axhline(y=0, color='r', linestyle='--', linewidth=1)
    
    # Trade Count
    ax6 = plt.subplot(3, 2, 6)
    ax6.bar([i - width/2 for i in assets], momentum_summary['trades'], width, label='Momentum', alpha=0.8)
    ax6.bar([i + width/2 for i in assets], mr_summary['trades'], width, label='Mean Reversion', alpha=0.8)
    ax6.set_title('Trade Count by Asset', fontsize=12, fontweight='bold')
    ax6.set_ylabel('# Trades')
    ax6.set_xticks(assets)
    ax6.set_xticklabels(momentum_summary['asset'])
    ax6.legend()
    ax6.grid(True, alpha=0.3, axis='y')
    
    plt.tight_layout()
    plt.savefig('hft_backtest_analysis.png', dpi=150, bbox_inches='tight')
    print("✓ Saved: hft_backtest_analysis.png")
    plt.show()

if __name__ == '__main__':
    try:
        base = Path('.')
        
        # Load data
        mom_eq = pd.read_csv(base / 'results_momentum_equity.csv')
        mr_eq = pd.read_csv(base / 'results_meanreversion_equity.csv')
        mom_sum = pd.read_csv(base / 'results_momentum_summary.csv')
        mr_sum = pd.read_csv(base / 'results_meanreversion_summary.csv')
        
        # Print summary statistics
        print("=" * 70)
        print("HFT BACKTESTER: COMPREHENSIVE ANALYSIS")
        print("=" * 70)
        print("\nMOMENTUM STRATEGY:")
        print(mom_sum.to_string(index=False))
        print(f"\nTotal Return: ${(mom_sum['final_equity'].sum() - 300000):,.2f}")
        print(f"Avg Sharpe: {mom_sum['sharpe'].mean():.4f}")
        print(f"Total Trades: {mom_sum['trades'].sum()}")
        
        print("\n" + "=" * 70)
        print("MEAN REVERSION STRATEGY:")
        print(mr_sum.to_string(index=False))
        print(f"\nTotal Return: ${(mr_sum['final_equity'].sum() - 300000):,.2f}")
        print(f"Avg Sharpe: {mr_sum['sharpe'].mean():.4f}")
        print(f"Total Trades: {mr_sum['trades'].sum()}")
        
        print("\n" + "=" * 70)
        print("PERFORMANCE COMPARISON:")
        print(f"Momentum wins: {(mom_sum['sharpe'] > mr_sum['sharpe']).sum()}/3 assets by Sharpe")
        print(f"Momentum total P&L: ${(mom_sum['final_equity'].sum() - 300000):,.2f}")
        print(f"MR total P&L: ${(mr_sum['final_equity'].sum() - 300000):,.2f}")
        print("=" * 70)
        
        # Plot
        plot_results(mom_eq, mr_eq, mom_sum, mr_sum)
        
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)
