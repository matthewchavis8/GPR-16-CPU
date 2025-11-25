# OS_STL - Operating System Standard Template Library for GPR-16 CPU

A lightweight, efficient standard library designed for the GPR-16 CPU architecture. Provides essential utilities for memory management, graphics, keyboard input, string manipulation, and mathematical operations.

## Table of Contents

- [Overview](#overview)
- [Installation](#installation)
- [Usage](#usage)
- [Modules](#modules)
  - [Math](#math-module)
  - [Memory](#memory-module)
  - [String](#string-module)
  - [Screen](#screen-module)
  - [Keyboard](#keyboard-module)
  - [System](#system-module)
- [Building](#building)
- [Testing](#testing)
- [Examples](#examples)

## Overview

OS_STL is a collection of utility classes optimized for the GPR-16 CPU's 16-bit architecture. It provides:

- **16-bit arithmetic operations** with overflow handling
- **32KB RAM simulation** with memory-mapped I/O
- **UTF-16 string management** with dynamic allocation
- **512×256 monochrome graphics** primitives
- **Memory-mapped keyboard** input handling
- **System utilities** for initialization and timing

## Installation

### Include in Your Project

To use OS_STL in your project, simply include the main header:

```cpp
#include <os_stl>
```

This single include gives you access to all modules:
- `GprMath` - Mathematical operations
- `GprMemory` - Memory management
- `GprString` - String manipulation
- `GprScreen` - Graphics primitives
- `GprKeyboard` - Keyboard input
- `GprSys` - System utilities

### CMake Integration

Add OS_STL as a subdirectory in your `CMakeLists.txt`:

```cmake
add_subdirectory(OS_STL)
target_link_libraries(your_project PRIVATE os_stl)
```

## Usage

### Basic Setup

Every program should initialize the system first:

```cpp
#include <os_stl>

int main() {
    GprSys::init();  // Initializes memory system
    
    // Your code here
    
    return 0;
}
```

## Modules

### Math Module

**Header:** `Modules/Math/gprMath.h`

Provides mathematical operations optimized for 16-bit signed integers (`Word`).

#### Functions

```cpp
// Absolute value
Word result = GprMath::abs(-10);  // Returns 10

// Multiplication (bit-shift algorithm)
Word product = GprMath::multiply(5, 10);  // Returns 50

// Division (returns 0 on divide-by-zero)
Word quotient = GprMath::divide(100, 5);  // Returns 20

// Min/Max
Word minimum = GprMath::min(10, 20);  // Returns 10
Word maximum = GprMath::max(10, 20);  // Returns 20

// Integer square root
Word sqrt_val = GprMath::sqrt(25);  // Returns 5
```

#### Example

```cpp
#include <os_stl>

Word calculate_distance(Word dx, Word dy) {
    Word dx2 = GprMath::multiply(dx, dx);
    Word dy2 = GprMath::multiply(dy, dy);
    return GprMath::sqrt(dx2 + dy2);
}
```

---

### Memory Module

**Header:** `Modules/Memory/gprMemory.h`

Manages 32KB of simulated RAM with memory-mapped I/O regions.

#### Memory Layout

| Address Range | Purpose |
|--------------|---------|
| 0 - 2047 | System/Reserved |
| 2048 - 16383 | Heap (available for allocation) |
| 16384 - 24575 | Screen memory-mapped region |
| 24576 | Keyboard register |
| 24577 - 32767 | Additional RAM |

#### Functions

```cpp
// Initialize memory (zeros all RAM, resets heap)
GprMemory::init();

// Read from memory
Word value = GprMemory::peek(1000);

// Write to memory
GprMemory::poke(1000, 42);

// Allocate memory (bump allocator, no deallocation)
Word address = GprMemory::alloc(100);  // Allocates 100 words

// Get raw memory pointer
Word* mem_ptr = GprMemory::data();
```

#### Example

```cpp
#include <os_stl>

int main() {
    GprSys::init();
    
    // Allocate buffer for 50 words
    Word buffer_addr = GprMemory::alloc(50);
    
    // Write data
    for (int i = 0; i < 50; i++) {
        GprMemory::poke(buffer_addr + i, i * 2);
    }
    
    // Read data
    Word value = GprMemory::peek(buffer_addr + 10);
    
    return 0;
}
```

---

### String Module

**Header:** `Modules/String/gprString.h`

Dynamic string class for UTF-16 character management.

#### Type

- `osChar` - 16-bit character type (`char16_t`)

#### Constructors

```cpp
// Default constructor (empty string)
GprString str1;

// From null-terminated buffer
GprString str2(u"Hello");

// Copy constructor
GprString str3(str2);

// Move constructor
GprString str4(std::move(str2));
```

#### Methods

```cpp
// Get length
size_t len = str.length();

// Get character at index
osChar ch = str.charAt(0);

// Set character at index
str.setCharAt(0, u'H');

// Append character
str.appendChar(u'!');

// Remove last character
str.eraseLastChar();

// Convert to integer
uint16_t num = str.intValue();  // Parses "123" -> 123

// Release memory
str.dispose();
```

#### Example

```cpp
#include <os_stl>

GprString name(u"GPR");
name.appendChar(u'-');
name.appendChar(u'1');
name.appendChar(u'6');
// name now contains "GPR-16"

GprString number(u"42");
uint16_t value = number.intValue();  // value = 42
```

---

### Screen Module

**Header:** `Modules/Screen/gprScreen.h`

Graphics primitives for the 512×256 monochrome display.

#### Constants

```cpp
GprScreen::WIDTH        // 512 pixels
GprScreen::HEIGHT       // 256 pixels
GprScreen::SCREEN_BASE  // 16384 (memory address)
```

#### Functions

```cpp
// Clear screen to white
GprScreen::clearScreen();

// Set drawing color (true=black, false=white)
GprScreen::setColor(true);

// Draw single pixel
GprScreen::drawPixel(100, 50);

// Draw line (Bresenham's algorithm)
GprScreen::drawLine(0, 0, 100, 100);

// Draw filled rectangle
GprScreen::drawRectangle(50, 50, 150, 100);

// Draw circle outline
GprScreen::drawCircle(256, 128, 50);
```

#### Coordinate System

- Origin (0, 0) is at **top-left**
- X-axis: 0-511 (left to right)
- Y-axis: 0-255 (top to bottom)

#### Example

```cpp
#include <os_stl>

int main() {
    GprSys::init();
    
    // Clear screen
    GprScreen::clearScreen();
    
    // Draw black shapes
    GprScreen::setColor(true);
    GprScreen::drawRectangle(10, 10, 100, 100);
    GprScreen::drawCircle(256, 128, 75);
    GprScreen::drawLine(0, 0, 511, 255);
    
    return 0;
}
```

---

### Keyboard Module

**Header:** `Modules/Keyboard/gprKeyboard.h`

Handles keyboard input through memory-mapped I/O at address 24576.

#### Special Character Codes

- `128` - Newline/Enter
- `129` - Backspace

#### Functions

```cpp
// Check current key (non-blocking, returns 0 if no key)
Word key = GprKeyboard::keyPressed();

// Wait for and read single character (blocking)
osChar ch = GprKeyboard::readChar();

// Read line of text (blocking, max 256 chars)
GprString line = GprKeyboard::readLine(u"Enter text: ");

// Read integer (blocking)
Word num = GprKeyboard::readInt(u"Enter number: ");
```

#### Example

```cpp
#include <os_stl>

int main() {
    GprSys::init();
    
    // Non-blocking check
    Word key = GprKeyboard::keyPressed();
    if (key != 0) {
        // Key is pressed
    }
    
    // Read user input
    GprString name = GprKeyboard::readLine(u"Name: ");
    Word age = GprKeyboard::readInt(u"Age: ");
    
    return 0;
}
```

---

### System Module

**Header:** `Modules/Sys/gprSys.h`

System utilities for initialization, timing, and control.

#### Functions

```cpp
// Initialize system (calls GprMemory::init)
GprSys::init();

// Halt execution (infinite loop)
GprSys::halt();

// Busy-wait delay in milliseconds
GprSys::wait(1000);  // Wait 1 second
```

#### Example

```cpp
#include <os_stl>

int main() {
    GprSys::init();
    
    while (true) {
        GprScreen::drawPixel(100, 100);
        GprSys::wait(500);  // Wait 500ms
        
        GprScreen::setColor(false);
        GprScreen::drawPixel(100, 100);
        GprSys::wait(500);  // Blink effect
    }
    
    GprSys::halt();  // Never returns
    return 0;
}
```

## Building

### Requirements

- CMake 3.14+
- C++17 compatible compiler
- GoogleTest (automatically fetched)

### Build Commands

```bash
# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build
cmake --build .

# Run tests
ctest --output-on-failure
```

### Build Targets

```bash
# Build everything
make all

# Build specific module tests
make unit_math
make unit_memory
make unit_string
make unit_screen
make unit_keyboard
make unit_sys
```

## Testing

All modules include comprehensive unit tests using GoogleTest.

### Running Tests

```bash
cd build

# Run all tests
ctest

# Run specific test suite
./Test/unit_math
./Test/unit_memory
./Test/unit_string
```

### Test Coverage

- ✅ Math: All operations (abs, multiply, divide, min, max, sqrt)
- ✅ Memory: Init, peek/poke, allocation, data access
- ✅ String: Constructors, operations, conversions
- ✅ Screen: Drawing primitives, color, bounds checking
- ✅ Keyboard: Key detection, special codes
- ✅ System: Initialization, timing

## Examples

### Complete Graphics Program

```cpp
#include <os_stl>

int main() {
    GprSys::init();
    GprScreen::clearScreen();
    GprScreen::setColor(true);
    
    // Draw grid
    for (int x = 0; x < 512; x += 50) {
        GprScreen::drawLine(x, 0, x, 255);
    }
    for (int y = 0; y < 256; y += 50) {
        GprScreen::drawLine(0, y, 511, y);
    }
    
    // Draw circles in corners
    GprScreen::drawCircle(50, 50, 30);
    GprScreen::drawCircle(462, 50, 30);
    GprScreen::drawCircle(50, 206, 30);
    GprScreen::drawCircle(462, 206, 30);
    
    GprSys::halt();
    return 0;
}
```

### Interactive Input Program

```cpp
#include <os_stl>

int main() {
    GprSys::init();
    
    GprString name = GprKeyboard::readLine(u"Enter your name: ");
    Word age = GprKeyboard::readInt(u"Enter your age: ");
    
    // Process input...
    
    return 0;
}
```

### Memory and String Management

```cpp
#include <os_stl>

int main() {
    GprSys::init();
    
    // Allocate array
    Word array_base = GprMemory::alloc(100);
    
    // Fill array
    for (int i = 0; i < 100; i++) {
        GprMemory::poke(array_base + i, i * i);
    }
    
    // String operations
    GprString message(u"Value: ");
    GprString num(u"42");
    
    uint16_t value = num.intValue();
    
    return 0;
}
```

## Architecture Notes

### Word Type

Throughout the library, `Word` is defined as `int16_t` (16-bit signed integer):
- Range: -32,768 to 32,767
- Used for all memory addresses and values

### Memory-Mapped I/O

The GPR-16 uses memory-mapped I/O:
- **Screen:** 16384-24575 (8192 words = 512×256 bits)
- **Keyboard:** 24576 (single word register)

### Design Principles

1. **Static Classes:** All functionality through static methods (no instances)
2. **constexpr:** Math functions are compile-time evaluable
3. **No Exceptions:** Designed for embedded/bare-metal systems
4. **Minimal Dependencies:** Only standard C++ library headers

## License

This project is part of the GPR-16 CPU ecosystem.

## Contributing

When contributing, ensure:
- All new functions have Doxygen comments
- Unit tests are provided for new functionality
- Code follows the existing style
- No external dependencies are introduced

---

**Version:** 1.0  
**Target:** GPR-16 CPU Architecture  
**Language:** C++17

