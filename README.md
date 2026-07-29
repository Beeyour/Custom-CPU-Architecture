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

- [x] **General-Purpose Registers & Register File:** Fully implemented.
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

- [🔄] **Custom C++ Assembler (`assembler.cpp`):** Functional base pipeline under active development to translate assembly mnemonics into Logisim-compatible memory files (`.hex`).

---

## ⚙️ Key Architectural Features

- **Datapath & Word Size:** 16-bit Data and Instruction Word
- **Execution Units:** Dedicated Data Movement, Arithmetic, Logic, Stack, and Control Flow execution paths
- **Status Flags:** `Z` (Zero), `S` (Sign), `C` (Carry), `O` (Overflow)
- **Simulation Platform:** Logisim Evolution
- **Software Pipeline:** Custom C++17 Assembler with `Makefile` automation

---

## 📜 Instruction Set Architecture (ISA) — Master Table

| Status | Legend                      |
| :----: | :-------------------------- |
| `[✓]`  | Implemented in Hardware     |
| `[ ]`  | Planned / Under Development |

---

### 1. Data Movement, Stack & I/O Group (`0x00` – `0x09`)

|  Status  | Opcode (Hex) | Instruction    | Description                               |
| :------: | :----------: | :------------- | :---------------------------------------- |
| `[✓][ ]` |    `0x00`    | `BOOT` / `NOP` | Pipeline initialization / No Operation    |
|  `[✓]`   |    `0x01`    | `LOAD`         | Read value from RAM to Register           |
|  `[✓]`   |    `0x02`    | `STOR`         | Write value from Register to RAM          |
|  `[✓]`   |    `0x03`    | `MOV`          | Copy value between Registers              |
|  `[✓]`   |    `0x04`    | `MOVI`         | Load Immediate 16-bit value into Register |
|  `[✓]`   |    `0x05`    | `PUSH`         | Push value onto Stack                     |
|  `[✓]`   |    `0x06`    | `POP`          | Pop value from Stack                      |
|  `[✓]`   |    `0x07`    | `XCHG`         | Exchange contents of two Registers        |
|  `[✓]`   |    `0x08`    | `INN`          | Read data from Input Port                 |
|  `[✓]`   |    `0x09`    | `OUTT`         | Write data to Output Port (Display)       |

---

### 2. ALU & Comparison Operations Group (`0x0A` – `0x27`)

| Status | Opcode (Hex) | Instruction | Description                                 |
| :----: | :----------: | :---------- | :------------------------------------------ |
| `[✓]`  |    `0x0A`    | `ADD`       | Add Register to Register                    |
| `[✓]`  |    `0x0B`    | `ADDI`      | Add Immediate value to Register             |
| `[✓]`  |    `0x0C`    | `ADDM`      | Add RAM memory content directly to Register |
| `[✓]`  |    `0x0D`    | `SUB`       | Subtract Register from Register             |
| `[✓]`  |    `0x0E`    | `SUBI`      | Subtract Immediate value from Register      |
| `[✓]`  |    `0x0F`    | `SUBM`      | Subtract RAM memory content from Register   |
| `[✓]`  |    `0x10`    | `MUL`       | Unsigned Multiplication (Register)          |
| `[✓]`  |    `0x11`    | `MULI`      | Unsigned Multiplication (Immediate)         |
| `[✓]`  |    `0x12`    | `DIV`       | Unsigned Division (Register)                |
| `[✓]`  |    `0x13`    | `DIVI`      | Unsigned Division (Immediate)               |
| `[✓]`  |    `0x14`    | `INC`       | Increment Register by 1                     |
| `[✓]`  |    `0x15`    | `DEC`       | Decrement Register by 1                     |
| `[✓]`  |    `0x16`    | `ANDD`      | Bitwise AND Registers                       |
| `[✓]`  |    `0x17`    | `ANDI`      | Bitwise AND Immediate                       |
| `[✓]`  |    `0x18`    | `ORR`       | Bitwise OR Registers                        |
| `[✓]`  |    `0x19`    | `ORI`       | Bitwise OR Immediate                        |
| `[✓]`  |    `0x1A`    | `XORR`      | Bitwise XOR Registers                       |
| `[✓]`  |    `0x1B`    | `XORI`      | Bitwise XOR Immediate                       |
| `[✓]`  |    `0x1C`    | `NOTT`      | Bitwise NOT (One's Complement)              |
| `[✓]`  |    `0x1D`    | `SHL`       | Logical Shift Left (Register)               |
| `[✓]`  |    `0x1E`    | `SHLI`      | Logical Shift Left (Immediate 4-bit)        |
| `[✓]`  |    `0x1F`    | `SHR`       | Logical Shift Right (Register)              |
| `[✓]`  |    `0x20`    | `SHRI`      | Logical Shift Right (Immediate 4-bit)       |
| `[✓]`  |    `0x21`    | `ROLL`      | Rotate Left (Register)                      |
| `[✓]`  |    `0x22`    | `ROLI`      | Rotate Left (Immediate 4-bit)               |
| `[✓]`  |    `0x23`    | `RORR`      | Rotate Right (Register)                     |
| `[✓]`  |    `0x24`    | `RORI`      | Rotate Right (Immediate 4-bit)              |
| `[✓]`  |    `0x25`    | `CMP`       | Compare Registers (Sets Flags)              |
| `[✓]`  |    `0x26`    | `CMPI`      | Compare Immediate (Sets Flags)              |
| `[✓]`  |    `0x27`    | `TEST`      | Bitwise Logical Compare (Sets Flags)        |

---

### 3. Control Flow & Branching Group (`0x28` – `0x35`)

| Status | Opcode (Hex) | Instruction   | Hardware Branch Condition               |
| :----: | :----------: | :------------ | :-------------------------------------- |
| `[ ]`  |    `0x28`    | `JMP`         | Unconditional Jump                      |
| `[ ]`  |    `0x29`    | `JZ` / `JE`   | Jump if Zero (`Z = 1`)                  |
| `[ ]`  |    `0x2A`    | `JNZ` / `JNE` | Jump if Not Zero (`Z = 0`)              |
| `[ ]`  |    `0x2B`    | `JC`          | Jump if Carry (`C = 1`)                 |
| `[ ]`  |    `0x2C`    | `JNC`         | Jump if Not Carry (`C = 0`)             |
| `[ ]`  |    `0x2D`    | `JS`          | Jump if Sign (`S = 1`)                  |
| `[ ]`  |    `0x2E`    | `JNS`         | Jump if Not Sign (`S = 0`)              |
| `[ ]`  |    `0x2F`    | `JA` / `JG`   | Jump if Greater / Above                 |
| `[ ]`  |    `0x30`    | `REF` / `JL`  | Jump if Less                            |
| `[ ]`  |    `0x31`    | `JAE`         | Jump if Greater or Equal                |
| `[ ]`  |    `0x32`    | `JBE`         | Jump if Less or Equal                   |
| `[ ]`  |    `0x33`    | `CALL`        | Call Subroutine (Push IP & Jump)        |
| `[ ]`  |    `0x34`    | `RET`         | Return from Subroutine (Pop IP)         |
| `[ ]`  |    `0x35`    | `LOOP`        | Decrement Register & Branch if Reg != 0 |

---

### 4. Flags & System Operations Group (`0x36` – `0x39`)

| Status | Opcode (Hex) | Instruction | Description                    |
| :----: | :----------: | :---------- | :----------------------------- |
| `[ ]`  |    `0x36`    | `CLC`       | Clear Carry Flag (`C = 0`)     |
| `[ ]`  |    `0x37`    | `STC`       | Set Carry Flag (`C = 1`)       |
| `[ ]`  |    `0x38`    | `CLR`       | Clear Register / Control Flags |
| `[ ]`  |    `0x39`    | `HALT`      | Halt CPU Clock & Pipeline      |

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
