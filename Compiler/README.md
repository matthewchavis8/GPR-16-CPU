## GPR-16 Compiler

A Jack-to-VM compiler targeting the GPR‑16 CPU platform, built on top of the Nand2Tetris toolchain.
This project takes Jack source programs and emits Nand2Tetris‑compatible VM code that can be run on the
VM emulator and, via the GPR‑16 stack machine, on the custom hardware.

---

### Architecture Overview

The compiler follows the classic front‑end pipeline:

- **Lexing**: `Tokenizer` reads a `.jack` source stream and produces a stream of typed tokens.
- **Parsing / Codegen**: `CompilationEngine` implements a recursive‑descent parser for Jack and emits VM code.
- **Symbol management**: `SymbolTable` tracks identifiers (type, kind, index) across class and subroutine scopes.
- **Orchestration**: `CompilerAnalyzer` owns the file streams and modules, validating inputs and invoking the pipeline.

At a high level:

```text
Jack source (.jack)
        │
        ▼
   Tokenizer  ──► tokens
        │
        ▼
CompilationEngine + SymbolTable
        │
        ▼
     VmWriter  ──► VM code (.vm)
```

---

### Key Components

- **`Modules/Tokenizer`**
  - `tokenizer.h`, `tokenizer.cpp`
  - Lexical analyzer for Jack:
    - Skips comments and whitespace.
    - Classifies tokens as `Keyword`, `Symbol`, `Identifier`, `IntConst`, `StringConst`.
    - Provides one‑token lookahead (`getNextToken`) and type‑safe accessors.

- **`Modules/CompilationEngine`**
  - `compileEngine.h`, `compileEngine.cpp`
  - Recursive‑descent compiler that:
    - Implements the Jack grammar (class, subroutines, var declarations, statements, expressions, terms).
    - Uses `SymbolTable` to resolve identifiers to VM segments and indices.
    - Emits Nand2Tetris VM code through `VmWriter`.

- **`Modules/VMWriter`**
  - `vmWriter.h`, `vmWriter.cpp`
  - Thin wrapper around an `std::ofstream` that:
    - Writes `push` / `pop` commands for all VM segments.
    - Writes arithmetic / logical commands (`add`, `sub`, `and`, `or`, `eq`, `lt`, `gt`, `neg`, `not`).
    - Writes labels, `goto`, `if-goto`, `call`, `function`, and `return`.

- **`Modules/SymbolTable`**
  - `symbolTable.h`, `symbolTable.cpp`
  - Per‑scope symbol management:
    - Tracks `static`, `field`, `arg`, and `var` identifiers.
    - Maintains type and running index per kind.
    - Provides queries for kind, type, and index of a given identifier.

- **`Modules/CompilerAnalyzer`**
  - `compilerAnalyzer.h`, `compilerAnalyzer.cpp`
  - High‑level façade:
    - Validates that the input path refers to a `.jack` file.
    - Owns the input (`std::ifstream`) and output (`std::ofstream`) streams.
    - Constructs and wires `Tokenizer`, `VmWriter`, and `CompilationEngine`.
    - Exposes a single `run()` method to compile Jack → VM.

- **`Modules/Utils`**
  - `keyword.h`, `tokenType.h` – Jack keyword and token type enums.
  - `segment.h`, `command.h` – VM segment and arithmetic command enums.
  - `identifier.h` – Identifier kind enum (`Static`, `Field`, `Arg`, `Var`, `None`).
  - `log.h` – Small helper that logs an error message to `stderr` and throws a typed exception.

---

### Features

- Full Jack front‑end: class, subroutine, variable declarations, expressions, and statements
  (`let`, `if`/`else`, `while`, `do`, `return`).
- Nand2Tetris‑compatible VM output:
  - Correct mapping of Jack variables and fields to VM segments (`static`, `this`, `argument`, `local`).
  - Support for constructor and method calling conventions (object allocation and `this` binding).
  - String construction via `String.new` / `String.appendChar`.
- Robust error reporting via `log.h` and `std::runtime_error` (and related exceptions).
- Unit tests for all core modules (`Tokenizer`, `SymbolTable`, `VMWriter`, `CompilationEngine`).

---

### Building and Running

From the project root (`Compiler/`):

- **Configure** (Debug or Release):

```bash
cmake --preset Debug
cmake --preset Release
```

- **Build**:

```bash
cmake --build Debug
cmake --build Release
```

- **Run the compiler**:

```bash
./Debug/Compiler path/to/File.jack
```

This produces a corresponding `path/to/File.vm` file containing the generated VM code.

- **Run tests** (from the chosen build dir):

```bash
ctest
ctest -R Tokenizer        # Filter by test suite / name
```

---

### Repository Structure (Compiler)

```text
Compiler/
├── CMakeLists.txt         # Top-level CMake project for the compiler
├── CMakePresets.json      # Presets for Debug / Release builds
├── compiler.cpp           # Main entry point (CLI front-end)
├── Modules/
│   ├── CompilerAnalyzer/
│   │   ├── compilerAnalyzer.h
│   │   └── compilerAnalyzer.cpp
│   ├── CompilationEngine/
│   │   ├── CMakeLists.txt
│   │   ├── compileEngine.h
│   │   └── compileEngine.cpp
│   ├── SymbolTable/
│   │   ├── CMakeLists.txt
│   │   ├── symbolTable.h
│   │   └── symbolTable.cpp
│   ├── Tokenizer/
│   │   ├── CMakeLists.txt
│   │   ├── tokenizer.h
│   │   └── tokenizer.cpp
│   ├── VMWriter/
│   │   ├── CMakeLists.txt
│   │   ├── vmWriter.h
│   │   └── vmWriter.cpp
│   └── Utils/
│       ├── command.h
│       ├── identifier.h
│       ├── keyword.h
│       ├── log.h
│       ├── segment.h
│       └── tokenType.h
└── Test/
    ├── CMakeLists.txt     # Test targets (GTest)
    ├── Tokenizer.cpp
    ├── VMWriter.cpp
    ├── symbolTable.cpp
    └── compilationEngine.cpp
```

---

### References

- *The Elements of Computing Systems* by Nisan & Schocken (Nand2Tetris)
- Nand2Tetris tools and VM specification (for target VM semantics)

