# Mark and Sweep Garbage Collector

## Description

This project is a simple implementation of a **Mark and Sweep Garbage Collector** in C. It demonstrates how memory management and automatic garbage collection can be handled in a custom virtual machine environment. The collector supports basic object types such as integers, floats, strings, arrays, and vector3 objects, and provides a clear step-by-step output of the garbage collection process.

## Why?

Manual memory management in C is error-prone and can lead to memory leaks or dangling pointers. This project aims to:

- **Demonstrate** the principles of garbage collection using the mark-and-sweep algorithm.
- **Provide** a learning resource.
- **Solve** the problem of automatic memory reallocation in environments where objects are dynamically created and referenced.

### Reference Counting

As a foundation, this project also includes a **Reference Counting (RefCount) algorithm**. RefCount is a very basic form of garbage collection, mainly used here for educational purposes. It helps illustrate the concept of tracking object references, but it is not robust enough for real-world use (e.g., it cannot handle cyclic references). The Mark and Sweep implementation builds upon this base to provide a more practical and complete solution.

## Quick Start

### Prerequisites

- [CMake](https://cmake.org/download/) installed and added to your system PATH.
- [MinGW](http://mingw-w64.org/) (or another C compiler) installed and added to your system PATH.

### Build Instructions

1. **Clone the repository** and navigate to the project directory:

   ```sh
   git clone <your-repo-url>
   cd GarbageCollector/Mark_and_Sweep
   ```

2. **Create and enter the build directory:**

   ```sh
   mkdir build
   cd build
   ```

3. **Generate build files and compile:**

   ```sh
   cmake -G "MinGW Makefiles" ..
   cmake --build .
   ```

4. **Run the executable:**
   ```sh
   GarbageCollector.exe
   ```

## Usage

- The main entry point is `testCases.c`, which runs several test cases to demonstrate garbage collection:

  - Creating and collecting integer, array, vector3, and string objects.
  - Removing references and triggering garbage collection.
  - Step-by-step output of the mark, trace, and sweep phases.

- **Example output:**

  ```
  === [GC] Starting garbage collection ===
  [GC] Mark phase...
  [GC][Mark] Marking root references in all frames...
  ...
  [GC] Trace phase...
  ...
  [GC] Sweep phase...
  ...
  === [GC] Garbage collection complete ===
  ```

- You can modify `testCases.c` to add your own objects and test scenarios.

---

Note: Project is still getting changes... so like
