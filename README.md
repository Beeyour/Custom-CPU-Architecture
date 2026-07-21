اجد صعوبة في الكتابه باللغة الانجليزية، على كل حال سينحصر استعمال الذكاء الاصطناعي في هذي الملفات التالية فقط
/README.md
/software/assembler/assembler.cpp

# 32-Bit Custom Processor Architecture

A custom 32-bit RISC-like processor designed in Logisim Evolution, featured with a custom 52-instruction Turing-complete Instruction Set Architecture (ISA), modular hardware components, and a custom C++ assembler toolchain.

---

## 📌 Key Architectural Features

- **Architecture:** 32-bit Datapath and Instruction Word
- **Instruction Set:** 52 Instructions (Arithmetic, Logic, Memory I/O, Control Flow, and Stack Operations)
- **ALU & Flags:** Supports Status Flags — `Z` (Zero), `C` (Carry), `S` (Sign), and `O` (Overflow)
- **Simulation Engine:** Logisim Evolution
- **Toolchain:** Custom C++17 Assembler with automated `Makefile` build system

---

## 📁 Repository Organization

    .
    ├── docs/
    │   └── opcode.md                  # Detailed 52-Instruction ISA Specification & Hex Encoding
    ├── hardware/
    │   └── cpu_32bit.circ             # Processor Circuit & Subcircuits (Logisim Evolution)
    ├── software/
    │   └── assembler/
    │       └── assembler.cpp          # C++17 Assembler Source Code
    ├── programs/
    │   ├── program.asm                # Assembly Source Code
    │   └── memory.hex                 # Generated Hex Output for Logisim RAM
    ├── Makefile                       # Automated Build Script
    ├── .gitignore                     # Git Exclusion Rules
    └── README.md                      # Project Overview

---

## 🛠️ Software Toolchain & Building

### Prerequisites

- **C++ Compiler:** `g++` with C++17 support
- **Build Tool:** `make`
- **Simulator:** Logisim Evolution

### Build Commands

- **Compile Assembler & Generate Hex File:**
  `make`

- **Clean Build Artifacts:**
  `make clean`

---

## 🚀 Execution & Simulation Workflow

1. **Write Code:** Edit or add Assembly code inside `programs/program.asm`.
2. **Assemble:** Run `make` to compile the C++ assembler and generate the `programs/memory.hex` file.
3. **Simulate:**
    - Open `hardware/cpu_32bit.circ` inside **Logisim Evolution**.
    - Right-click the **RAM Module** -> Select **Load Image**.
    - Choose `programs/memory.hex` and start the clock generator.

---

## 📖 ISA Documentation

For complete opcode mappings, register layouts, and control signal specifications, refer to `docs/opcode.md`.
