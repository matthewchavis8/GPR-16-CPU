# Logic Gates in HDL

This repository contains my implementations of the logic gates and chips in HDL

## Overview

The goal of this project is to build a full set of basic logic gates, multiplexors, and demultiplexors from a single primitive: the NAND gate. These components form the foundation for all higher-level hardware in subsequent projects.

## Implemented Chips

### **Elementary Gates**
- `Not` – Inverts a single bit using NAND.
- `And` – Logical AND implemented from NAND and NOT.
- `Or` – Logical OR implemented via De Morgan's law using NAND.
- `Xor` – Exclusive OR built from basic gates.
- `Mux` – 2-way data selector.
- `DMux` – 1-to-2 demultiplexer.

### **Multi-bit Gates**
- `Not16` – 16-bit NOT.
- `And16` – 16-bit AND.
- `Or16` – 16-bit OR.
- `Mux16` – 16-bit 2-way multiplexer.

### **Multi-way Gates**
- `Or8Way` – Outputs `1` if any of 8 input bits is `1`.
- `Mux4Way16` – Selects one of 4 16-bit inputs based on a 2-bit selector.
- `Mux8Way16` – Selects one of 8 16-bit inputs based on a 3-bit selector.
- `DMux4Way` – Routes input to one of 4 outputs.
- `DMux8Way` – Routes input to one of 8 outputs.

## How to Run

1. Open the **Hardware Simulator** provided in the Nand2Tetris tools.
2. Load any `.hdl` file from this project.
3. Load the corresponding `.tst` test script and `.cmp` compare file from the course materials.
4. Run the test to verify correctness.

## Notes

- All gates are built hierarchically starting from the `Nand` gate.
- These chips will be reused in later to build arithmetic components like the ALU.
---
