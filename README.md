# GPR-16 CPU

A custom 16-bit CPU built from first principles using the hardware description language (HDL).  
This project implements a fully functional computer architecture with general-purpose registers, memory, screen, and keyboard I/O — all running in the Nand2Tetris Hardware Simulator.

## Architecture Overview

The **GPR-16 CPU** is inspired by the Hack CPU from *The Elements of Computing Systems* (Nand2Tetris), but extended with a more flexible register file and richer instruction support.

Key Components:
- **Logic Gates (`LogicGates/`)** – Primitive gates (`Not`, `And`, `Or`, `Mux`, etc.) that form the foundation.
- **Combinational Chips (`CombinationalChips/`)** – ALU, multiplexers, adders, etc.
- **Sequential Chips (`SequentialChips/`)** – Registers, PC, RAM hierarchy (RAM8, RAM64, RAM512, RAM4K, RAM16K).
- **Top-Level CPU** – Fetch–decode–execute cycle, connected to memory, screen, and keyboard.
- **I/O Memory Map**
  - **Screen**: Writes to `0x4000–0x5FFF` update pixels in the hardware simulator.
  - **Keyboard**: Reads from `0x6000` to get the last pressed key code.

## Features

- 16-bit General Purpose Register File (GPR): Enables direct load/store to multiple registers.
- Instruction Set: Supports arithmetic, logical, memory, and jump operations.
- Memory-Mapped I/O: Full support for screen drawing and keyboard input via the Nand2Tetris simulator.
- Extensible Design: Easily add new instructions or registers by modifying the decoder and control logic.
- Assembly Programs: Example `.asm` programs are provided in the `programs/` folder.

## Writing to Registers

Registers are accessed via instructions encoded in your assembly program.  
For example, to write a value to `R1`:

```asm
@value
D=A       // Load value into D-register
@R1
M=D       // Store into register R1
````

This behaves similarly to Hack assembly but uses the register file as a memory-mapped region.

## Running in the Nand2Tetris Hardware Simulator

1. Open the CPU

   * Launch the Hardware Simulator from the [Nand2Tetris tools](https://www.nand2tetris.org/software).
   * Load `SequentialChips/CPU.hdl`.

2. Load a Program

   * In the simulator, go to `File → Load Program...`.
   * Select one of the assembly files in `programs/`.

3. Run

   * Click `Run` or step through instructions manually.
   * Watch the screen memory update live as the program draws pixels or outputs text.

## Architecture Diagram (Simplified)

```text
          ┌────────────┐
          │   ROM      │  (Instruction Memory)
          └─────┬──────┘
                │
           ┌────▼─────┐
           │  CPU     │
           │ (Control │
           │  + ALU)  │
           └────┬─────┘
                │
        ┌───────┼─────────┐
        │       │         │
   ┌────▼───┐ ┌─▼─────┐ ┌─▼──────┐
   │Memory  │ │Screen │ │Keyboard│
   └────────┘ └───────┘ └────────┘
```

## Repository Structure

```
GPR-16-CPU/
├── LogicGates/          # Basic gates (And, Or, Not, etc.)
├── CombinationalChips/  # ALU, multiplexers, adders
├── SequentialChips/     # Registers, PC, RAM hierarchy, top-level CPU
└── programs/            # Sample assembly programs

```
## References

* [The Elements of Computing Systems](https://www.nand2tetris.org/book) by Nisan & Schocken
* [Nand2Tetris Tools](https://www.nand2tetris.org/software)
