#!/usr/bin/env python3
import csv
import os
import subprocess
import sys

def run_cpp_benchmark(n, queries, results_dir, csv_path):
    os.makedirs(results_dir, exist_ok=True)
    subprocess.run(["make", "build/bench_filters"], check=True)
    result = subprocess.run(
        ["./build/bench_filters", str(n), str(queries)],
        capture_output=True,
        text=True,
        check=True,
    )
    if result.stderr:
        print(result.stderr, file=sys.stderr)
    with open(csv_path, "w") as f:
        f.write(result.stdout)


def load_rows(csv_path):
    with open(csv_path) as f:
        return list(csv.DictReader(f))


def print_table(rows):
    header = f"{'Filter':<8} {'Target FPR':>10} {'Emp. FPR':>10} {'Bits/item':>10} {'Insert(ns)':>11} {'Hit(ns)':>9} {'Miss(ns)':>9}"
    print(header)
    print("-" * len(header))
    for r in rows:
        print(
            f"{r['filter']:<8} "
            f"{100 * float(r['target_fpr']):>9.4f}% "
            f"{100 * float(r['empirical_fpr']):>9.4f}% "
            f"{float(r['bits_per_item']):>10.2f} "
            f"{float(r['insert_ns']):>11.1f} "
            f"{float(r['lookup_hit_ns']):>9.1f} "
            f"{float(r['lookup_miss_ns']):>9.1f}"
        )


def write_dat(rows, filter_name, path):
    with open(path, "w") as f:
        for r in rows:
            if r["filter"] == filter_name:
                f.write(
                    f"{r['target_fpr']} {r['empirical_fpr']} {r['bits_per_item']} "
                    f"{r['insert_ns']} {r['lookup_hit_ns']} {r['lookup_miss_ns']}\n"
                )


def run_gnuplot(script):
    subprocess.run(["gnuplot"], input=script, text=True, check=True)


def generate_plots(rows, n, results_dir):
    bloom_dat = os.path.join(results_dir, "bloom.dat")
    cuckoo_dat = os.path.join(results_dir, "cuckoo.dat")
    write_dat(rows, "bloom", bloom_dat)
    write_dat(rows, "cuckoo", cuckoo_dat)

    common = """
set terminal pngcairo size 800,500 enhanced font 'Sans,11'
set grid
set logscale x
set xrange [*:*] reverse
set key top left
"""

    # 1. Bits per item vs Empirical FPR
    run_gnuplot(common + f"""
set output '{results_dir}/bits_vs_fpr.png'
set title 'Espaco vs Taxa de Falsos Positivos (n = {n})'
set xlabel 'Taxa de falsos positivos empirica (escala log)'
set ylabel 'Bits por elemento'
plot '{bloom_dat}' using 2:3 with linespoints lw 2 pt 7 title 'Bloom Filter', \\
     '{cuckoo_dat}' using 2:3 with linespoints lw 2 pt 5 title 'Cuckoo Filter (b=4)'
""")

    # 2. Insertion time vs Target FPR
    run_gnuplot(common + f"""
set output '{results_dir}/insert_time.png'
set title 'Tempo medio de insercao (n = {n})'
set xlabel 'Taxa de falsos positivos alvo (escala log)'
set ylabel 'Tempo por insercao (ns)'
plot '{bloom_dat}' using 1:4 with linespoints lw 2 pt 7 title 'Bloom Filter', \\
     '{cuckoo_dat}' using 1:4 with linespoints lw 2 pt 5 title 'Cuckoo Filter (b=4)'
""")

    # 3. Lookup time (hit and miss) vs Target FPR
    run_gnuplot(common + f"""
set output '{results_dir}/lookup_time.png'
set title 'Tempo medio de consulta (n = {n})'
set xlabel 'Taxa de falsos positivos alvo (escala log)'
set ylabel 'Tempo por consulta (ns)'
plot '{bloom_dat}' using 1:5 with linespoints lw 2 pt 7 title 'Bloom (hit)', \\
     '{bloom_dat}' using 1:6 with linespoints lw 2 dt 2 pt 6 title 'Bloom (miss)', \\
     '{cuckoo_dat}' using 1:5 with linespoints lw 2 pt 5 title 'Cuckoo (hit)', \\
     '{cuckoo_dat}' using 1:6 with linespoints lw 2 dt 2 pt 4 title 'Cuckoo (miss)'
""")

    os.remove(bloom_dat)
    os.remove(cuckoo_dat)


if __name__ == "__main__":
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 120000
    queries = int(sys.argv[2]) if len(sys.argv) > 2 else 500000
    results_dir = os.path.join("benchmarks/results", f"n_{n}")
    csv_path = os.path.join(results_dir, "comparison.csv")

    run_cpp_benchmark(n, queries, results_dir, csv_path)
    rows = load_rows(csv_path)
    print_table(rows)
    generate_plots(rows, n, results_dir)
    print(f"\nCSV e graficos salvos em {results_dir}/")
