# Multithreaded File Sorter – Operating Systems Final Project

This project was developed as the final assignment for the Operating Systems course at UNICAMP by **Renan Felipe Rodrigues** and **Nicolas Lourenço Mafei**.

The objective is to build a **modular and recursive program in C** that supports a **dynamic number of threads and input files**. Each thread is responsible for reading and sorting a subset of input files using QuickSort. The sorted arrays are then merged into a single output file.

---

## Features

- Dynamic handling of any number of threads and input files
- Thread-level performance measurement using `clock_gettime`
- Modular file structure with separation of concerns
- Sorting based on QuickSort (adapted from GeeksforGeeks)
- Final merging step implements a merge logic across sorted arrays

---

## File Structure

- `main.c` — Initializes threads, manages time tracking, and writes the final sorted result.
- `funcoes.h` — Header file defining the thread data structure and function prototypes.
- `sort_alg.c` — Implements the QuickSort algorithm (source: [GeeksforGeeks](https://www.geeksforgeeks.org/quick-sort-in-c/)).
- `thread_func.c` — Thread function responsible for reading, storing, and sorting integers from assigned files.
- `unificar.c` — Merges the sorted arrays from all threads into a single ordered array.
- `Makefile` — Compile automation.

---

## Compilation and Execution

Ensure all files are in the same directory, then run:

```bash
make
```  
```bash
./mergesort num_threads file1.dat file2.dat ... fileN.dat -o output.dat
```
