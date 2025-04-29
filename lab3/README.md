# Matrix-Vector Multiplication Benchmark

This project benchmarks the performance of a matrix-matrix multiplication program using multiple matrix sizes and thread counts.

## Description

The provided Bash script automates the following tasks:

1. Compiles the source code for matrix and vector generation (`geraMatrizBinario.c`) and for matrix-matrix multiplication (`matvet.c`).
2. Generates binary matrix and vector files with increasing dimensions.
3. Executes the compiled matrix-vector multiplication program (`a.out`) for each combination of:
   - Matrix  size (`500` to `20000`, but CSV contains results **up to 3000x3000**),
   - Number of threads (`1` to `4`),
   - Repetitions (`10` per configuration).

All results are stored in `saida.csv`.

> ⚠️ **Note**: The CSV (`saida.csv`) only includes execution times for matrix sizes **up to 3000x3000**. Larger matrices (`5000`, `10000`, `20000`) are generated but not benchmarked in the current script output.

---

## How to Use

### Prerequisites

- A Unix-based environment (Linux, macOS, or WSL)
- GCC compiler with `pthread` support

### Steps

1. Save the Bash script (e.g., `script.sh`) and make it executable:
   ```bash
   chmod +x script.sh
   ./script.sh
