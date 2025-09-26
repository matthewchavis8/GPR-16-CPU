# Hack Assembler

A C++ implementation of an assembler for the Hack machine language, part of the GPR-16-CPU project. This assembler translates Hack assembly programs (.asm files) into binary machine code (.hack files) that can be executed on the Hack computer platform.

## Overview

The Hack Assembler implements a two-pass assembler that handles the complete Hack assembly language specification, including:

- A-instructions (addressing instructions): `@value`
- C-instructions (compute instructions): `dest=comp;jump`
- Label declarations: `(LABEL)`
- Symbolic references to variables and labels
- Predefined symbols (SP, LCL, ARG, THIS, THAT, R0-R15, SCREEN, KBD)

## Architecture

The assembler is built using a modular architecture with the following components:

### Core Modules

- **Parser Module** (`Modules/Parser/`)
  - Parses assembly language commands
  - Breaks commands into component fields and symbols
  - Removes whitespace and comments
  - Provides convenient access to command components

- **Code Module** (`Modules/Code/`)
  - Translates Hack assembly mnemonics into binary codes
  - Handles dest, comp, and jump field translations
  - Implements the binary encoding specification

- **SymbolTable Module** (`Modules/SymbolTable/`)
  - Manages correspondence between symbolic labels and numeric addresses
  - Handles predefined symbols, labels, and variables
  - Implements symbol resolution for two-pass assembly

- **Utils Module** (`Modules/Utils/`)
  - Contains common utilities and type definitions
  - Defines `CommandType` enumeration (A_COMMAND, C_COMMAND, L_COMMAND)

- **MainDriver Module** (`Modules/MainDriver/`)
  - Main program that orchestrates the translation process
  - Handles file I/O and command-line interface

### Command Types

The assembler recognizes three types of assembly commands:

- `A_COMMAND`: Address instructions (`@symbol` or `@number`)
- `C_COMMAND`: Compute instructions (`dest=comp;jump`)
- `L_COMMAND`: Label pseudo-commands (`(LABEL)`)

## Building

The project uses CMake for build configuration. Each module has its own CMakeLists.txt file for modular compilation.

### Prerequisites

- C++17 compatible compiler
- CMake 3.10 or higher

### Build Instructions

```bash
# From the project root directory
cd Assembler
mkdir build
cd build
cmake ..
make
```

## Usage

```bash
./Assembler program.asm
```

This will read `program.asm` and generate `program.hack` containing the translated binary machine code.

### Input Format

- Assembly files should have `.asm` extension
- Support for comments (lines starting with `//`)
- Whitespace is ignored
- Case-sensitive symbols (convention: UPPERCASE for labels, lowercase for variables)

### Output Format

- Binary files have `.hack` extension
- Each line contains exactly 16 binary digits (0s and 1s)
- Represents one 16-bit machine language instruction

## Symbol Handling

The assembler implements a two-pass symbol resolution process:

### First Pass
- Builds symbol table without generating code
- Records ROM addresses for label symbols
- Maintains instruction counter for proper address mapping

### Second Pass
- Translates all instructions to binary
- Resolves symbolic references using the symbol table
- Allocates RAM addresses for variables (starting at address 16)

### Predefined Symbols

| Symbol | Value | Description |
|--------|-------|-------------|
| SP | 0 | Stack pointer |
| LCL | 1 | Local segment base |
| ARG | 2 | Argument segment base |
| THIS | 3 | This segment base |
| THAT | 4 | That segment base |
| R0-R15 | 0-15 | General purpose registers |
| SCREEN | 16384 | Screen memory map base |
| KBD | 24576 | Keyboard memory map |
