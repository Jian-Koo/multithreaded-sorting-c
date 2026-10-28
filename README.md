# Multithreaded Sorting in C

An implementation of **odd-even transposition sorting using POSIX threads (Pthreads)** in C, developed to explore multithreading, thread synchronisation, mutual exclusion, condition variables, and shared-memory programming.

The program uses two threads to perform alternating sorting passes on a shared integer array, coordinating their execution through a mutex and condition variable. It also tracks the number of swaps performed by each thread.

## Project Overview

Odd-even transposition sort is a comparison-based sorting algorithm that repeatedly performs two alternating phases:

- **Even phase:** Compares adjacent elements at indices (0, 1), (2, 3), (4, 5), and so on.
- **Odd phase:** Compares adjacent elements at indices (1, 2), (3, 4), (5, 6), and so on.

During each phase, adjacent elements are swapped if they are in the wrong order.

The process continues until two consecutive phases complete without any swaps, indicating that the array is sorted.

This implementation assigns the two phases to separate threads, using synchronisation mechanisms to coordinate access to shared data.

### Key Features

- Multithreaded implementation using POSIX threads
- Odd-even transposition sorting algorithm
- Shared-memory communication between threads
- Mutex-based mutual exclusion
- Condition variables for thread coordination
- Alternating thread execution
- Per-thread and total swap tracking
- Dynamic thread information structures
- File-based integer input
- Optional random integer generator

## Implementation

### Thread Architecture

The program creates two worker threads, **T1** and **T2**, which operate on the same integer array.

| Thread | Responsibility |
|---|---|
| T1 | Performs even-indexed comparisons, starting at index 0 |
| T2 | Performs odd-indexed comparisons, starting at index 1 |

Both threads execute the same `sort()` function, with their behaviour determined by their assigned logical thread IDs.

Each thread receives a `ThreadInfo` structure containing the information required for execution, including:

- Logical thread ID
- Shared integer array and its size
- Individual swap counter
- Shared turn variable
- Shared consecutive no-swap counter
- Pointer to the mutex
- Pointer to the condition variable

### Thread Synchronisation

The implementation uses two POSIX synchronisation primitives:

**Mutex (`pthread_mutex_t`)**

Protects shared data and ensures that only one thread executes a sorting phase at a time.

**Condition Variable (`pthread_cond_t`)**

Allows a thread to wait until it is permitted to execute its sorting phase.

The shared `turn` variable determines which thread may proceed.

The synchronisation sequence is:

1. A thread acquires the mutex.
2. If it is not the thread's turn, it waits on the condition variable.
3. Once permitted, the thread performs its sorting phase.
4. The thread updates its swap counter and the shared no-swap counter.
5. It assigns the next turn to the other thread.
6. It signals the condition variable and releases the mutex.
7. Both threads continue alternating until the termination condition is satisfied.

The use of condition variables avoids continuously polling the shared turn variable while waiting.

### Termination Condition

The algorithm uses a shared `noSwapCount` variable to track consecutive sorting phases without swaps.

- If a phase performs no swaps, the counter increases.
- If a phase performs any swaps, the counter resets to zero.
- When two consecutive phases perform no swaps, sorting is complete.

Both threads terminate, and the main thread waits for them using `pthread_join()` before displaying the final results.

### Execution Model

Although the implementation uses two threads, the sorting phases are deliberately executed in alternating order.

The mutex and turn-based condition variable prevent the two threads from sorting simultaneously.

Therefore, the implementation primarily demonstrates **thread creation, synchronisation, coordination, and shared-memory management**, rather than parallel sorting performance or speedup.

## Repository Structure

```text
multithreaded-sorting-c/
├── README.md
├── .gitignore
├── Makefile
├── sss.c
├── SortingFunction.c
├── SortingFunction.h
└── IntegerGenerator.c
```

| File | Description |
|---|---|
| `sss.c` | Main program handling input, thread creation, synchronisation initialisation, and result reporting |
| `SortingFunction.c` | Sorting algorithm, thread information initialisation, integer swapping, and array printing |
| `SortingFunction.h` | ThreadInfo structure, constants, and function declarations |
| `IntegerGenerator.c` | Optional utility for generating random integer input files |
| `Makefile` | Build configuration for compiling the sorting application |

## Requirements

- GCC or a compatible C compiler
- POSIX-compatible environment with Pthreads support
- Make

The project is designed for Unix-like operating systems, including Linux.

No additional third-party libraries are required.

## Compilation

Compile the sorting application using the provided Makefile:

```bash
make
```

This produces the executable:

```text
sss
```

The Makefile compiles `sss.c` and `SortingFunction.c` and links them into the final executable.

## Usage

### 1. Prepare an Input File

Create a text file containing space-separated integers.

For example, `numbers.txt`:

```text
9 2 7 3 1 6
```

The program supports an integer array of up to 200 elements.

### 2. Run the Sorting Application

Execute the program with the input filename:

```bash
./sss numbers.txt
```

The application reads the integers, creates two sorting threads, and waits for both threads to finish.

### 3. Example Output

```text
Initial Array :
9 2 7 3 1 6

Thread ID 139787524212416 : total number of swaps = 6
Thread ID 139787515819712 : total number of swaps = 4

Sorted Array :
1 2 3 6 7 9

Total swaps : 10
```

The thread IDs are assigned by the operating system and will vary between executions.

The output shows the original array, the number of swaps performed by each thread, the sorted array, and the combined swap count.

## Random Integer Generator

The project includes `IntegerGenerator.c`, an optional utility for creating input files containing random integers.

### Compilation

Compile the generator separately:

```bash
gcc -Wall -pedantic -Werror IntegerGenerator.c -o IntegerGenerator
```

### Generate Integers

Run the generator and specify an output filename:

```bash
./IntegerGenerator numbers.txt
```

The program prompts for the number of integers to generate:

```text
Amount of integer (between 0 to 200) >>
```

For example:

```text
Amount of integer (between 0 to 200) >> 10
10 integers have been written to 'numbers.txt'.
```

The generator creates between 0 and 200 random integers, with values ranging from 0 to 299.

The generated file can then be used with the sorting application:

```bash
./sss numbers.txt
```

## Cleaning Build Files

Remove the sorting executable and object files using:

```bash
make clean
```

The optional `IntegerGenerator` executable is compiled separately and is not removed by this Makefile target.

## Design Considerations and Limitations

### Synchronised Execution

The use of two threads does not imply simultaneous execution of sorting passes.

The algorithm explicitly alternates between threads to demonstrate controlled access to shared memory.

### Shared State

The array, turn variable, and termination counter are shared between threads.

The mutex and condition variable coordinate access to these shared resources, while each thread maintains its own swap counter.

### Input Constraints

The implementation uses a fixed-size array supporting up to 200 integers.

The input parser assumes appropriately formatted integer data. It is not designed for arbitrary or malformed input files.

### Algorithmic Complexity

Odd-even transposition sort has quadratic worst-case time complexity, O(n²).

The algorithm is suitable for demonstrating adjacent-element comparisons and synchronisation, but it is not intended to compete with more efficient general-purpose sorting algorithms.

### Portability

The implementation relies on POSIX threading APIs and is intended for environments supporting Pthreads.

It is not designed as a native Windows threading application.

## Project Background

This project was originally developed as part of undergraduate operating systems coursework and has been organised for portfolio presentation.

The primary objective was to explore practical multithreaded programming in C, focusing on the coordination of multiple threads operating on shared memory.

Through the implementation of odd-even transposition sorting, the project demonstrates:

- POSIX thread creation and lifecycle management
- Mutual exclusion and condition variable synchronisation
- Shared-memory coordination
- Thread-specific state management
- Race-condition prevention through controlled execution
- Thread termination and resource cleanup
- Modular C programming

The emphasis is on understanding operating system concurrency mechanisms and applying them to a working sorting algorithm.

## Copyright

Copyright © 2026 Jian Heng Koo. All rights reserved.

The original source code, documentation, and other original materials in this repository are the work of Jian Heng Koo.

No licence is granted to copy, modify, redistribute, publish, sublicense, or use these materials without permission from the author.
