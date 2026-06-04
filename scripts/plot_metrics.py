import pandas as pd
import matplotlib.pyplot as plt
import subprocess
import os
import sys

def plot_graph(csv_path, output_png, title):
    TOTAL_ACCESSES = 10000
    
    try:
        df = pd.read_csv(csv_path)
    except FileNotFoundError:
        print(f"Error: {csv_path} not found. C++ engine might have failed.")
        return

    # Convert Faults to Hit Rate Percentage
    df['FIFO_HitRate'] = ((TOTAL_ACCESSES - df['FIFO']) / TOTAL_ACCESSES) * 100
    df['LRU_HitRate'] = ((TOTAL_ACCESSES - df['LRU']) / TOTAL_ACCESSES) * 100
    df['Clock_HitRate'] = ((TOTAL_ACCESSES - df['Clock']) / TOTAL_ACCESSES) * 100
    df['Optimal_HitRate'] = ((TOTAL_ACCESSES - df['Optimal']) / TOTAL_ACCESSES) * 100

    # Create the Plot
    plt.figure(figsize=(10, 6))
    plt.plot(df['FrameSize'], df['Optimal_HitRate'], label='Optimal', color="#250fcf", linewidth=2, linestyle='--', marker='s', markevery=5)
    plt.plot(df['FrameSize'], df['LRU_HitRate'], label='LRU', color='#2ca02c', linewidth=2, marker='^', markevery=5)
    plt.plot(df['FrameSize'], df['Clock_HitRate'], label='Clock', color='#ff7f0e', linewidth=2, linestyle='--', marker='s', markevery=5)
    plt.plot(df['FrameSize'], df['FIFO_HitRate'], label='FIFO', color='#d62728', linewidth=2, marker='o', markevery=5)

    plt.title(title, fontsize=14, fontweight='bold')
    plt.xlabel("Cache Size (Blocks/Frames)", fontsize=12)
    plt.ylabel("Hit Rate (%)", fontsize=12)
    plt.ylim(0, 100)
    plt.xlim(0, 100)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend(loc='lower right', fontsize=12, frameon=True, shadow=True)

    plt.tight_layout()
    plt.savefig(output_png, dpi=300)
    plt.close() # Close memory to prevent overlap
    print(f"Saved Graph: {output_png}")

if __name__ == "__main__":
    # Define our workspace paths
    root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    benchmark_exe = os.path.join(root_dir, "benchmark")
    data_dir = os.path.join(root_dir, "data")

    # Check if the C++ engine actually exists
    if not os.path.exists(benchmark_exe):
        print("C++ Executable 'benchmark' not found! Please compile it first.")
        sys.exit(1)

    # Our master list of all 4 workloads
    workloads = [
        {
            "trace": "random_trace.txt", 
            "csv": "results_random.csv", 
            "png": "Graph_1_Random.png", 
            "title": "Pure Random Chaos: Hit Rate vs. Cache Size"
        },
        {
            "trace": "standard_locality_trace.txt", 
            "csv": "results_standard.csv", 
            "png": "Graph_2_Standard_Locality.png", 
            "title": "Standard 80/20 Database: Hit Rate vs. Cache Size"
        },
        {
            "trace": "high_locality_trace.txt", 
            "csv": "results_high.csv", 
            "png": "Graph_3_High_Locality.png", 
            "title": "High Locality (Tight Loop): Hit Rate vs. Cache Size"
        },
        {
            "trace": "cyclic_trace.txt", 
            "csv": "results_cyclic.csv", 
            "png": "Graph_4_Cyclic.png", 
            "title": "Cyclic Workload: Hit Rate vs. Cache Size"
        }
    ]

    print("Booting Master Automation Pipeline...\n")

    for wl in workloads:
        trace_path = os.path.join(data_dir, wl["trace"])
        csv_path = os.path.join(data_dir, wl["csv"])
        png_path = os.path.join(data_dir, wl["png"])

        if not os.path.exists(trace_path):
            print(f"Skipping {wl['trace']} (File not found. Did you run generate_trace.py?)")
            continue

        # 1. Command C++ to crunch the numbers
        print(f"C++ Crunching data for: {wl['trace']}...")
        subprocess.run([benchmark_exe, trace_path, csv_path], check=True)

        # 2. Command Python to plot the results
        plot_graph(csv_path, png_path, wl["title"])
        print("-" * 50)

    print("Pipeline Complete! Check your 'data/' folder for all the graphs.")