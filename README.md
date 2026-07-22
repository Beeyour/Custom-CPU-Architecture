اجد صعوبة في الكتابه باللغة الانجليزية، على كل حال سينحصر استعمال الذكاء الاصطناعي في هذي الملفات التالية فقط
/README.md
/software/assembler/assembler.cpp
/Makefile

# 16-Bit Custom Processor Architecture

A custom 16-bit RISC-like processor designed in **Logisim Evolution**, featuring a modular hardware architecture, dedicated boot logic, hardware status flags, and an extensible custom C++ toolchain.

---

## 🏗️ Hardware Implementation & Development Status

### 1. Hardware Components

- [x] **General-Purpose Registers & Register File:** Fully implemented.
- [x] **Memory System:** ROM Boot module and RAM integration completed.
- [🔄] **Control Unit:** Core decoder and main control logic implemented; remaining instruction signals in active development.
- [🔄] **ALU Status Flags:** Flag logic under development (`Z` - Zero, `S` - Sign, `C` - Carry, `O` - Overflow).

### 2. Software Toolchain

- [🔄] **Custom C++ Assembler (`assembler.cpp`):** Functional base pipeline under active development to translate assembly mnemonics into Logisim-compatible memory files (`.hex`).

---

## ⚙️ Key Architectural Features

- **Datapath & Word Size:** 16-bit Data and Instruction Word
- **Execution Units:** Dedicated Data Movement, Arithmetic, Logic, Stack, and Control Flow execution paths
- **Status Flags:** `Z` (Zero), `S` (Sign), `C` (Carry), `O` (Overflow)
- **Simulation Platform:** Logisim Evolution
- **Software Pipeline:** Custom C++17 Assembler with `Makefile` automation

---

## 📜 Instruction Set Architecture (ISA)

Below is the planned instruction set derived from the decoder architecture. Instructions marked with **`[✓]`** are fully implemented in hardware.

### 🟢 Implemented Instructions (`11/50+`)

| Category          | Instruction  | Description                                         |
| :---------------- | :----------- | :-------------------------------------------------- |
| **System**        | `BOOT` `[✓]` | Initializes execution pipeline from ROM Boot memory |
| **Memory / Data** | `LOAD` `[✓]` | Load value from memory to register                  |
|                   | `STOR` `[✓]` | Store value from register to memory                 |
|                   | `MOV` `[✓]`  | Move value between registers                        |
|                   | `MOVI` `[✓]` | Move immediate value into register                  |
| **Arithmetic**    | `ADD` `[✓]`  | Add values of two registers                         |
|                   | `ADDI` `[✓]` | Add immediate value to register                     |
|                   | `ADDM` `[✓]` | Add memory content directly to register             |
|                   | `SUB` `[✓]`  | Subtract register value from another                |
|                   | `SUBI` `[✓]` | Subtract immediate value from register              |
|                   | `SUBM` `[✓]` | Subtract memory content from register               |

---

### 🟡 Planned & In-Maintenance Instructions

- **System & Stack:** `NOP`, `PUSH`, `POP`, `XCHG`, `INN`, `OUTT`, `CLR`, `HALT`
- **Arithmetic & Extension:** `MUL`, `DIV`, `INC`, `DEC`
- **Logic & Bitwise:** `ANDD`, `ANDI`, `ORR`, `ORI`, `XORR`, `XORI`, `NOTT`, `SHL`, `SHR`, `ROLL`, `RORR`
- **Comparisons & Flags:** `CMP`, `CMPI`, `TEST`, `CLC`, `STC`
- **Control Flow & Branching:** `JMP`, `JZ` (`JE`), `JNZ` (`JNE`), `JC`, `JNC`, `JS`, `JNS`, `JA` (`JG`), `REF` (`JL`), `JAE`, `JBE`, `CALL`, `RET`

---

## 📁 Repository Organization

    .
    ├── docs/
    │   └── opcode.md                  # Detailed ISA Specification & Opcode Encodings
    ├── hardware/
    │   └── cpu_16bit.circ             # 16-bit Processor Architecture (Logisim Evolution)
    ├── software/
    │   └── assembler/
    │       └── assembler.cpp          # Custom C++17 Assembler Source Code
    ├── programs/
    │   ├── program.asm                # Assembly Test Program
    │   └── memory.hex                 # Compiled Hex Output for Logisim RAM
    ├── Makefile                       # Automated Build System
    ├── .gitignore                     # Git Exclusion Rules
    └── README.md                      # Project Overview

---

## 🛠️ Software Toolchain & Build Guide

### Prerequisites

- **C++ Compiler:** `g++` (C++17 or higher)
- **Build Automation:** `make`
- **Logic Simulator:** Logisim Evolution

### Compilation Commands

Build the assembler and generate the memory file:

    make

Clean build outputs:

    make clean

---

## 🚀 Execution & Simulation Workflow

1. **Write Assembly:** Write target instructions inside `programs/program.asm`.
2. **Compile:** Run `make` to invoke the C++ assembler and output `programs/memory.hex`.
3. **Load Image:**
    - Open `hardware/cpu_16bit.circ` in **Logisim Evolution**.
    - Right-click the **RAM / ROM Module** -> **Load Image**.
    - Select `programs/memory.hex` and start the clock oscillator.
