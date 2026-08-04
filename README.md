> **AI Usage & Verification Disclaimer**
> AI assistance was strictly limited to English language translation, document formatting, and code/toolchain structuring across the following files only:
>
> - `/docs/opcode.md`
> - `/software/assembler/assembler.cpp`
> - `/Makefile`
> - `/README.md`
> - `/package.json` & `/syntaxes/` (VS Code custom syntax extension)
>
> All AI-generated outputs and suggestions were thoroughly reviewed, audited, and verified to ensure complete technical accuracy with the hardware design.

# 16-Bit Custom Processor Architecture

A custom 16-bit RISC-like processor designed in **Logisim Evolution**, featuring a modular hardware architecture, dedicated boot logic, hardware status flags, and an extensible custom C++ toolchain.

---

## 🏗️ Hardware Implementation & Development Status

### 1. Hardware Components

- [x] **General-Purpose Registers & Register File:** Fully implemented (`R0`–`R6`, `SP`, `FLAG`).
- [x] **Memory System:** ROM Boot module and RAM integration completed.
- [x] **ALU Core & Datapath Execution Units:** All Arithmetic, Logical, Shift, Rotate, and Comparison execution paths completed.
- [x] **ALU Status Flags:**
    - [x] **Zero Flag (`Z`):** Fully implemented.
    - [x] **Sign Flag (`S`):** Fully implemented.
    - [x] **Carry Flag (`C`):** Fully implemented.
    - [x] **Overflow Flag (`O`):** Fully implemented.
- [🔄] **Control Unit & Micro-Operations:** Core opcode decoder established; micro-operation sequencing and control signals across execution units in active development.
- [ ] **Control Flow & System Operations Unit:** Branching logic, hardware `LOOP`, subroutine call/return stack, and system control circuitry pending implementation.

### 2. Software Toolchain

- [x] **Custom C++ Assembler (`assembler.cpp`):** Completed two-pass assembler supporting 16-bit word generation, base-offset memory expressions (e.g., `[R2 + 10b + 10h]`), implicit `SP` register encodings, 4-bit immediate shifts, and Logisim Evolution v3.0 `.hex` memory image formatting.

---

## ⚙️ Key Architectural Features

- **Datapath & Word Size:** 16-bit Data and Instruction Word (Multi-word support for 16-bit immediates and addresses)
- **Execution Units:** Dedicated Data Movement, Arithmetic, Logic, Stack, and Control Flow execution paths
- **Status Flags:** `Z` (Zero), `S` (Sign), `C` (Carry), `O` (Overflow)
- **Registers:** 8 Addressable Registers (`R0`–`R6` General Purpose, `SP` Stack Pointer, `FLAG` Status Register)
- **Simulation Platform:** Logisim Evolution
- **Software Pipeline:** Custom C++17 Assembler with `Makefile` automation

---

## 📜 Instruction Set Architecture (ISA) — Master Table

| Status | Legend                      |
| :----: | :-------------------------- |
| `[✓]`  | Implemented in Hardware     |
| `[ ]`  | Planned / Under Development |

---

### 1. Data Movement, Stack & I/O Group (`0x00` – `0x0B`)

| Status | Opcode (Hex) | Instruction    | Description                                     |
| :----: | :----------: | :------------- | :---------------------------------------------- |
| `[✓]`  |    `0x00`    | `BOOT` / `NOP` | Pipeline initialization / No Operation          |
| `[✓]`  |    `0x01`    | `LOAD`         | Read value from Direct RAM Address to Register  |
| `[✓]`  |    `0x02`    | `STOR`         | Write value from Register to Direct RAM Address |
| `[✓]`  |    `0x03`    | `LAD`          | Load Register with Base + Offset Addressing     |
| `[✓]`  |    `0x04`    | `STR`          | Store Register with Base + Offset Addressing    |
| `[✓]`  |    `0x05`    | `MOV`          | Copy value between Registers                    |
| `[✓]`  |    `0x06`    | `MOVI`         | Load Immediate 16-bit value into Register       |
| `[✓]`  |    `0x07`    | `PUSH`         | Push value onto Stack (Implicit `SP` Encoding)  |
| `[✓]`  |    `0x08`    | `POP`          | Pop value from Stack (Implicit `SP` Encoding)   |
| `[ ]`  |    `0x09`    | `XCHG`         | Exchange contents of two Registers              |
| `[ ]`  |    `0x0A`    | `INN`          | Read data from Input Port                       |
| `[ ]`  |    `0x0B`    | `OUTT`         | Write data to Output Port (Display)             |

---

### 2. ALU & Comparison Operations Group (`0x0C` – `0x29`)

| Status | Opcode (Hex) | Instruction | Description                                 |
| :----: | :----------: | :---------- | :------------------------------------------ |
| `[ ]`  |    `0x0C`    | `ADD`       | Add Register to Register                    |
| `[ ]`  |    `0x0D`    | `ADDI`      | Add Immediate value to Register             |
| `[ ]`  |    `0x0E`    | `ADDM`      | Add RAM memory content directly to Register |
| `[ ]`  |    `0x0F`    | `SUB`       | Subtract Register from Register             |
| `[ ]`  |    `0x10`    | `SUBI`      | Subtract Immediate value from Register      |
| `[ ]`  |    `0x11`    | `SUBM`      | Subtract RAM memory content from Register   |
| `[ ]`  |    `0x12`    | `MUL`       | Unsigned Multiplication (Register)          |
| `[ ]`  |    `0x13`    | `MULI`      | Unsigned Multiplication (Immediate)         |
| `[ ]`  |    `0x14`    | `DIV`       | Unsigned Division (Register)                |
| `[ ]`  |    `0x15`    | `DIVI`      | Unsigned Division (Immediate)               |
| `[ ]`  |    `0x16`    | `INC`       | Increment Register by 1                     |
| `[ ]`  |    `0x17`    | `DEC`       | Decrement Register by 1                     |
| `[ ]`  |    `0x18`    | `ANDD`      | Bitwise AND Registers                       |
| `[ ]`  |    `0x19`    | `ANDI`      | Bitwise AND Immediate                       |
| `[ ]`  |    `0x1A`    | `ORR`       | Bitwise OR Registers                        |
| `[ ]`  |    `0x1B`    | `ORI`       | Bitwise OR Immediate                        |
| `[ ]`  |    `0x1C`    | `XORR`      | Bitwise XOR Registers                       |
| `[ ]`  |    `0x1D`    | `XORI`      | Bitwise XOR Immediate                       |
| `[ ]`  |    `0x1E`    | `NOTT`      | Bitwise NOT (One's Complement)              |
| `[ ]`  |    `0x1F`    | `SHL`       | Logical Shift Left (Register)               |
| `[ ]`  |    `0x20`    | `SHLI`      | Logical Shift Left (Immediate 4-bit)        |
| `[ ]`  |    `0x21`    | `SHR`       | Logical Shift Right (Register)              |
| `[ ]`  |    `0x22`    | `SHRI`      | Logical Shift Right (Immediate 4-bit)       |
| `[ ]`  |    `0x23`    | `ROLL`      | Rotate Left (Register)                      |
| `[ ]`  |    `0x24`    | `ROLI`      | Rotate Left (Immediate 4-bit)               |
| `[ ]`  |    `0x25`    | `RORR`      | Rotate Right (Register)                     |
| `[ ]`  |    `0x26`    | `RORI`      | Rotate Right (Immediate 4-bit)              |
| `[ ]`  |    `0x27`    | `CMP`       | Compare Registers (Sets Flags)              |
| `[ ]`  |    `0x28`    | `CMPI`      | Compare Immediate (Sets Flags)              |
| `[ ]`  |    `0x29`    | `TEST`      | Bitwise Logical Compare (Sets Flags)        |

---

### 3. Control Flow & Branching Group (`0x2A` – `0x37`)

| Status | Opcode (Hex) | Instruction   | Hardware Branch Condition               |
| :----: | :----------: | :------------ | :-------------------------------------- |
| `[ ]`  |    `0x2A`    | `JMP`         | Unconditional Jump                      |
| `[ ]`  |    `0x2B`    | `JZ` / `JE`   | Jump if Zero (`Z = 1`)                  |
| `[ ]`  |    `0x2C`    | `JNZ` / `JNE` | Jump if Not Zero (`Z = 0`)              |
| `[ ]`  |    `0x2D`    | `JC`          | Jump if Carry (`C = 1`)                 |
| `[ ]`  |    `0x2E`    | `JNC`         | Jump if Not Carry (`C = 0`)             |
| `[ ]`  |    `0x2F`    | `JS`          | Jump if Sign (`S = 1`)                  |
| `[ ]`  |    `0x30`    | `JNS`         | Jump if Not Sign (`S = 0`)              |
| `[ ]`  |    `0x31`    | `JA` / `JG`   | Jump if Greater / Above                 |
| `[ ]`  |    `0x32`    | `REF` / `JL`  | Jump if Less                            |
| `[ ]`  |    `0x33`    | `JAE`         | Jump if Greater or Equal                |
| `[ ]`  |    `0x34`    | `JBE`         | Jump if Less or Equal                   |
| `[ ]`  |    `0x35`    | `CALL`        | Call Subroutine (Push IP & Jump)        |
| `[ ]`  |    `0x36`    | `RET`         | Return from Subroutine (Pop IP)         |
| `[ ]`  |    `0x37`    | `LOOP`        | Decrement Register & Branch if Reg != 0 |

---

### 4. Flags & System Operations Group (`0x38` – `0x3B`)

| Status | Opcode (Hex) | Instruction | Description                    |
| :----: | :----------: | :---------- | :----------------------------- |
| `[ ]`  |    `0x38`    | `CLC`       | Clear Carry Flag (`C = 0`)     |
| `[ ]`  |    `0x39`    | `STC`       | Set Carry Flag (`C = 1`)       |
| `[ ]`  |    `0x3A`    | `CLR`       | Clear Register / Control Flags |
| `[ ]`  |    `0x3B`    | `HALT`      | Halt CPU Clock & Pipeline      |

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
