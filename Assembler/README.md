# GPR-16 Assembler

A two-pass assembler for the Hack assembly language, translating `.asm` source files into binary machine code (`.hack`) for the Hack computer platform as specified in the Nand2Tetris.

## Project Overview

This assembler processes Hack assembly programs, handling:
- **A-instructions**: Direct addressing (`@value` or `@symbol`)
- **C-instructions**: Computation, destination, and jump operations (`dest=comp;jump`)
- **Labels**: Symbolic references for program flow (`(LABEL)`)

The assembler resolves symbols, manages a symbol table with predefined and user-defined variables, and generates 16-bit binary instructions.

## Repository Structure

```
Assembler/
├── Modules/
│   ├── Parser/          # Tokenizes and classifies assembly commands ✓
│   ├── Code/            # Translates mnemonics to binary (in progress)
│   ├── SymbolTable/     # Manages symbol addresses (in progress)
│   ├── MainDriver/      # Orchestrates two-pass assembly (in progress)
│   └── Utils/           # Shared types and utilities
├── Test/                # GoogleTest unit harness
├── CMakeLists.txt       # Build configuration
└── CMakePresets.json    # Debug and Release build presets
```

## Build Instructions

This project uses CMake presets for Debug and Release configurations.

### Debug Build
```bash
cmake --preset=Debug
cmake --build --preset=Debug
```

### Release Build
```bash
cmake --preset=Release
cmake --build --preset=Release
```

Executables are generated in `build/` (default), `Debug/`, or `Release/` depending on the preset.

## Running the Assembler

Once built, run the assembler with:

```bash
./Assembler path/to/program.asm
```

The assembler generates a `.hack` file in the same directory as the input, containing 16-bit binary machine code instructions.

**Example:**
```bash
./Assembler examples/Add.asm
# Output: examples/Add.hack
```

## Debug vs Release Modes

- **Debug**: Includes symbols and assertions for development and testing.
- **Release**: Optimized binary with `-O3` for production use.

## Testing

Unit tests are implemented with GoogleTest and run via CTest.

### Run All Tests
```bash
ctest --preset=Debug
```

### Run Specific Test Suite
```bash
cd build/test
./unit_Parser
```
