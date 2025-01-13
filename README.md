# 8-Point FFT Implementation using HLS

This project implements an 8-point Fast Fourier Transform (FFT) algorithm using High-Level Synthesis (HLS) tools. It demonstrates how to efficiently design and optimize hardware for signal processing tasks. The project includes:
- HLS-based C++ source code with pragmas for optimization.
- Testbench for functional verification.
- Resource utilization and performance analysis.

## Features
- Fully synthesizable HLS design.
- Optimized for low latency using pipelining and loop unrolling.
- Verified against Python-generated reference results.

## Getting Started
1. Clone the repository and open the project in Vivado HLS.
2. Run the provided scripts to synthesize the design and generate hardware.

## Results
- Resource utilization: BRAM = 2, DSP = 4.
- Maximum clock frequency: 200 MHz.
