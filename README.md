# GPR-16 CPU

A complete custom 16-bit computer system built from first principles, featuring a general-purpose register (GPR) based CPU and full software toolchain.

## Overview

This project extends the Nand2Tetris curriculum into a complete computing platform. Starting from basic logic gates, it implements a custom 16-bit CPU architecture with general-purpose registers, then builds the entire software stack above it: a high-level language compiler, VM translator, assembler, and operating system layer.

Unlike the original Hack computer, the GPR-16 CPU uses a more flexible register file architecture, enabling more efficient code generation and richer instruction support. Every component—from NAND gates to the compiler—was built as part of this project, demonstrating the complete path from hardware to high-level software.

## Architecture Overview

### CPU & Register Model

The GPR-16 CPU implements a 16-bit Harvard architecture with:

- **General-Purpose Register File**: Multiple 16-bit registers for flexible data manipulation
- **16-bit Data Bus**: Single-cycle memory and register operations
- **ALU**: Full arithmetic and logical operations (add, subtract, AND, OR, NOT, etc.)
- **Program Counter**: Supports conditional and unconditional jumps
- **Instruction Decoder**: Decodes and executes both A-instructions and C-instructions

### Memory & I/O

The system uses a unified memory-mapped I/O architecture:

- **RAM**: `0x0000–0x3FFF` (16K words of general-purpose memory)
- **Screen Buffer**: `0x4000–0x5FFF` (8K words, 256×512 pixel display)
- **Keyboard**: `0x6000` (memory-mapped keyboard input)

Writes to screen memory immediately update the display in the Nand2Tetris Hardware Simulator. The keyboard register reflects the currently pressed key's ASCII code.

## Project Structure

The repository is organized into hardware, toolchain, operating system, and demonstration layers:

```text
GPR-16-CPU/
├── LogicGates/          # Primitive gates (NAND, AND, OR, NOT, XOR, MUX, DMUX)
├── CombinationalChips/  # Combinational logic (ALU, adders, multiplexers)
├── SequentialChips/     # Memory elements (registers, RAM8, RAM64, RAM512, RAM4K, RAM16K)
├── System/              # Top-level integration (CPU, Memory, Computer)
├── Compiler/            # High-level language (Jack-style) to VM compiler
├── VM-Translator/       # VM code to assembly translator
├── Assembler/           # Assembly to machine code assembler
├── OS_STL/              # Operating system and standard library (Math, Memory, Screen, Keyboard, String, Sys)
└── programs/            # Example assembly programs
```

### Hardware Layer

**LogicGates/**  
Foundation of the system. Implements elementary logic gates from NAND primitives: `Not`, `And`, `Or`, `Xor`, `Mux`, `DMux`, and their 16-bit/multi-way variants.

**CombinationalChips/**  
Stateless computational units built from logic gates: half adder, full adder, 16-bit adder (`Add16`), incrementer (`Inc16`), and the full ALU supporting 18 different operations.

**SequentialChips/**  
Clocked memory elements: 1-bit register (`Bit`), 16-bit register (`Register`), program counter (`PC`), and RAM hierarchy (`RAM8` → `RAM64` → `RAM512` → `RAM4K` → `RAM16K`).

**System/**  
Top-level integration. `CPU.hdl` implements the fetch-decode-execute cycle with register file integration. `Memory.hdl` combines RAM, screen buffer, and keyboard into a unified address space. `Computer.hdl` connects CPU, memory, and ROM into a complete, bootable system.

### Toolchain Layer

**Compiler/**  
Translates high-level Jack language (object-oriented, similar to Java) into stack-based VM code. Handles expressions, control flow, functions, and object-oriented constructs. Built in C++ with modular components: tokenizer, symbol table, compilation engine, and VM writer.

**VM-Translator/**  
Converts VM code (stack-based intermediate representation) into GPR-16 assembly language. Implements stack operations, arithmetic/logical commands, memory segment access, branching, and function call/return mechanisms. Designed specifically for the GPR-16 instruction set.

**Assembler/**  
Two-pass assembler that translates symbolic assembly into 16-bit machine code. Handles symbols, labels, variables, and both A-instructions and C-instructions. Outputs binary `.hack` files loadable into the ROM of the Hardware Simulator.

### Operating System Layer

**OS_STL/**  
Provides fundamental services and abstractions for programs:

- **Math**: Multiplication, division, square root, min, max
- **Memory**: Heap allocation (`alloc`, `deAlloc`), memory manipulation
- **String**: String creation, manipulation, conversion utilities
- **Screen**: Graphics primitives (draw pixel, line, rectangle, circle)
- **Keyboard**: Input handling and character reading
- **Sys**: System initialization, halt, wait, and error handling

Implemented in C++ to match the semantics expected by Jack programs.

### Programs

**programs/**  
Demonstration programs showcasing the system's capabilities:

- **Mult.asm**: Software multiplication using repeated addition
- **Fill.asm**: Interactive screen fill program responding to keyboard input

These programs can be loaded directly into the Hardware Simulator to run on the CPU.

**Step-by-step execution:**

1. **Compiler** parses Jack source code, performs semantic analysis, and generates VM instructions
2. **VM-Translator** converts VM commands into GPR-16 assembly, managing the stack and memory segments
3. **Assembler** resolves symbols and translates assembly mnemonics into 16-bit machine instructions
4. **ROM** loads the machine code as the instruction memory
5. **CPU** fetches instructions, decodes them, executes ALU operations, and updates registers/memory
6. **Memory** routes addresses to RAM, screen buffer, or keyboard based on the memory map
7. **Screen** renders pixel changes in real-time as memory-mapped writes occur
8. **Keyboard** provides input by updating the memory-mapped keyboard register

## How to Run

### Running the Hardware Simulator

1. **Download the Nand2Tetris Software Suite**  
   Download the Hardware Simulator from the [Nand2Tetris tools page](https://www.nand2tetris.org/software).

2. **Load the Computer**  
   Launch the Hardware Simulator and load `System/Computer.hdl`. This chip integrates the CPU, memory, and I/O into a complete system.

3. **Load a Program**  
   - In the simulator, navigate to `View → ROM` (or `File → Load Program`)
   - Select an assembly program from `programs/` (e.g., `Fill.asm`)
   - The assembler built into the simulator will convert it to machine code and load it into ROM

4. **Run the Program**  
   - Click `Run` to execute continuously, or use `Step` to advance one instruction at a time
   - Open `View → Screen` to see the output display
   - Open `View → Keyboard` to provide input

5. **Monitor Execution**  
   - Watch registers update in real-time
   - Observe memory addresses change as the program executes
   - See the program counter advance through instructions

### Using the Toolchain (Optional)

If you want to assemble programs manually or test the full toolchain:

**Assembler:**
```bash
cd Assembler/
cmake -B build -S . --preset Debug
cmake --build build
./build/bin/Assembler <input.asm> <output.hack>
```

**VM-Translator:**
```bash
cd VM-Translator/
cmake -B build -S . --preset Debug
cmake --build build
./build/bin/VM-Translator <input.vm> <output.asm>
```

**Compiler:**
```bash
cd Compiler/
cmake -B build -S . --preset Debug
cmake --build build
./build/bin/Compiler <input.jack> <output.vm>
```

## Assembly Language Example

Registers are accessed through memory-mapped locations. Here's how to store a value in a register:

```asm
@42       // A = 42
D=A       // D = A (D now holds 42)
@R1       // A = address of R1
M=D       // Memory[R1] = D (store 42 in register R1)
```

Conditional jump example:

```asm
@R0       // Load address of R0
D=M       // D = Memory[R0]
@POSITIVE // A = address of POSITIVE label
D;JGT     // Jump to POSITIVE if D > 0
@ZERO     // A = address of ZERO label
0;JMP     // Unconditional jump to ZERO
(POSITIVE)
    // Code for positive case
(ZERO)
    // Code for zero/negative case
```

## Architecture Diagram

```text
                    ┌──────────────────┐
                    │   ROM (32K)      │
                    │ Instruction Mem  │
                    └────────┬─────────┘
                             │
                    ┌────────▼─────────┐
                    │   CPU (GPR-16)   │
                    │  ┌─────────────┐ │
                    │  │   Decoder   │ │
                    │  └──────┬──────┘ │
                    │  ┌──────▼──────┐ │
                    │  │     ALU     │ │
                    │  └──────┬──────┘ │
                    │  ┌──────▼──────┐ │
                    │  │  Registers  │ │
                    │  └─────────────┘ │
                    └────────┬─────────┘
                             │
              ┌──────────────┼──────────────┐
              │              │              │
     ┌────────▼────────┐ ┌───▼────┐ ┌──────▼──────┐
     │   RAM (16K)     │ │ Screen │ │  Keyboard   │
     │  0x0000-0x3FFF  │ │  8K    │ │   0x6000    │
     └─────────────────┘ └────────┘ └─────────────┘
```

## Learning Goals

This project demonstrates:

- **Digital Logic Design**: Building a complete CPU from primitive gates using hardware description language
- **Computer Architecture**: Instruction set design, fetch-decode-execute cycle, memory hierarchy, register files
- **Compiler Construction**: Lexical analysis, parsing, semantic analysis, code generation, symbol tables
- **Virtual Machines**: Stack-based intermediate representations, bytecode translation
- **Systems Programming**: Assembly language, two-pass assemblers, symbol resolution, binary encoding
- **Operating Systems**: Memory management, I/O abstraction, standard library implementation
- **Full-Stack Integration**: Understanding how every layer of abstraction connects, from transistors to high-level code

## Technical Specifications

**CPU**
- Architecture: 16-bit Harvard
- Register File: General-purpose registers (GPR-based)
- ALU: 18 operations (arithmetic, logical, bit manipulation)
- Instruction Format: A-instructions (address/immediate) and C-instructions (compute)
- Clock: Single-cycle execution (synchronous)

**Memory**
- Total Address Space: 32K words (15-bit addressing)
- RAM: 16K words (0x0000-0x3FFF)
- Screen: 8K words (0x4000-0x5FFF, 256×512 pixels, 1-bit color)
- Keyboard: 1 word (0x6000)

**Instruction Set**
- A-instruction: `@value` (load 15-bit constant or address)
- C-instruction: `dest=comp;jump` (compute, store, and/or jump)
- Supports: arithmetic, logical, memory access, conditional/unconditional branching

## References

- [The Elements of Computing Systems](https://www.nand2tetris.org/book) by Noam Nisan & Shimon Schocken
- [Nand2Tetris Course](https://www.nand2tetris.org/)
- [Nand2Tetris Software Tools](https://www.nand2tetris.org/software)

## License

This project was created for educational purposes as an extension of the Nand2Tetris curriculum.
