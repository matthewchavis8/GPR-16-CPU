## GPR-16 VM Translator

A stack-based virtual machine translator for the GPR-16 CPU platform. This tool converts Nand2Tetris-compatible VM code (`.vm` files) into GPR-16 assembly language (`.asm` files), bridging the gap between high-level compilation and machine code execution.

---

### Architecture Overview

The VM translator follows a classic two-stage translation pipeline:

- **Parsing**: `Parser` reads a `.vm` source stream and produces a sequence of typed VM commands with arguments.
- **Code Generation**: `CodeWriter` emits GPR-16 assembly instructions corresponding to each VM command.
- **Orchestration**: `VMTranslator` coordinates file I/O, wires the parser and code writer, and handles both single-file and directory translation.

At a high level:

Note: add image here VM translator flow

---

### Key Components

- **`Modules/Parser`**
  - `parser.h`, `parser.cpp`
  - VM command parser that:
    - Reads VM source files line-by-line, skipping comments and blank lines.
    - Identifies command types: `C_ARITHMETIC`, `C_PUSH`, `C_POP`, `C_LABEL`, `C_GOTO`, `C_IF`, `C_FUNCTION`, `C_CALL`, `C_RETURN`.
    - Extracts command arguments (`arg1`, `arg2`) for subsequent processing.
    - Provides `hasMoreLines()` and `advance()` for iterating through commands.

- **`Modules/CodeWriter`**
  - `codeWriter.h`, `codeWriter.cpp`
  - Assembly code generator that:
    - Translates VM arithmetic/logical operations (`add`, `sub`, `neg`, `eq`, `gt`, `lt`, `and`, `or`, `not`) into GPR-16 assembly.
    - Implements memory segment access for `push`/`pop` commands across all VM segments:
      - `local`, `argument`, `this`, `that` (pointer-based segments)
      - `temp` (fixed base address)
      - `static` (file-scoped variables)
      - `pointer` (manipulates `this`/`that` pointers)
      - `constant` (immediate values)
    - Generates branching code for `label`, `goto`, and `if-goto`.
    - Implements function call/return mechanism with proper stack frame management.
    - Maintains label uniqueness using internal counters and function context.

- **`Modules/VMTranslator`**
  - `vmtranslator.h`, `vmtranslator.cpp`
  - High-level orchestrator:
    - Validates input paths (single `.vm` file or directory of `.vm` files).
    - Constructs `Parser` for each input file and a single `CodeWriter` for output.
    - Iterates through all VM commands, dispatching to appropriate `CodeWriter` methods.
    - Handles directory-level translation by processing multiple files into one assembly output.

- **`Modules/Utils`**
  - `CommandType.h` – Enumeration of VM command types used throughout the translator.

---

### Features

- **Complete VM Command Set**:
  - Arithmetic/logical operations: `add`, `sub`, `neg`, `eq`, `gt`, `lt`, `and`, `or`, `not`
  - Stack manipulation: `push` and `pop` across all memory segments
  - Program flow: `label`, `goto`, `if-goto`
  - Function operations: `function`, `call`, `return`

- **GPR-16 Assembly Output**:
  - Optimized assembly tailored to the GPR-16 instruction set.
  - Proper stack pointer management and calling conventions.
  - Efficient segment addressing for pointer-based and fixed segments.

- **File and Directory Support**:
  - Translate individual `.vm` files or entire directories containing multiple `.vm` files.
  - Automatic output file naming (`input.vm` → `input.asm` or `directory` → `directory.asm`).

- **Robust Error Handling**:
  - Validates command types and arguments.
  - Reports malformed VM code with contextual error messages.

- **Unit Tested**:
  - Comprehensive test coverage for `Parser` and `CodeWriter` using GoogleTest.

---

### Building and Running

From the project root (`VM-Translator/`):

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

- **Run the VM translator on a single file**:

```bash
./Debug/VM-Translator path/to/File.vm
```

This produces a corresponding `path/to/File.asm` file containing GPR-16 assembly code.

- **Run on a directory** (translates all `.vm` files into one `.asm` file):

```bash
./Debug/VM-Translator path/to/directory/
```

This produces `path/to/directory/directory.asm`.

- **Specify custom output file**:

```bash
./Debug/VM-Translator input.vm output.asm
```

- **Run tests**:

```bash
cd Debug
ctest
ctest -R parser        # Filter by test name
ctest -R codeWriter    # Test code generation
```

---

### VM Language Overview

The VM language is a stack-based intermediate representation with the following command categories:

#### Arithmetic/Logical Commands
```
add      // Pop two values, push their sum
sub      // Pop two values, push difference (second - first)
neg      // Pop one value, push its negation
eq       // Pop two values, push -1 (true) if equal, 0 otherwise
gt       // Pop two values, push -1 if second > first, 0 otherwise
lt       // Pop two values, push -1 if second < first, 0 otherwise
and      // Pop two values, push bitwise AND
or       // Pop two values, push bitwise OR
not      // Pop one value, push bitwise NOT
```

#### Memory Access Commands
```
push segment index    // Push value from segment[index] onto stack
pop segment index     // Pop value from stack into segment[index]
```

**Segments**: `local`, `argument`, `this`, `that`, `temp`, `static`, `pointer`, `constant`

#### Program Flow Commands
```
label LABEL           // Declare a label
goto LABEL            // Unconditional jump to label
if-goto LABEL         // Pop value; jump to label if not zero
```

#### Function Commands
```
function f n          // Declare function f with n local variables
call f m              // Call function f with m arguments
return                // Return from current function
```

---

### Translation Example

**Input VM Code (`SimpleAdd.vm`):**
```
push constant 7
push constant 8
add
```

**Output Assembly (`SimpleAdd.asm`):**
```asm
// push constant 7
@7
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 8
@8
D=A
@SP
A=M
M=D
@SP
M=M+1

// add
@SP
AM=M-1
D=M
A=A-1
M=D+M
```

The generated assembly pushes constants onto the stack, then pops two values, adds them, and pushes the result back.

---