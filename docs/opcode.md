# 16-Bit Processor Instruction Set & Control Signal Specification

This document details the hardware control signals, register bit encodings, and binary word layouts for the 16-bit custom processor architecture.

---

## 🎛️ 1. Control Signal Line Mappings (C0 – C63)

The Control Unit outputs a 64-bit control line array (`C0` to `C63`) to orchestrate data movement across internal buses during execution steps.

| Control Line Range | Target Module / Function | Description                                             |
| :----------------- | :----------------------- | :------------------------------------------------------ |
| **`C0`**           | **ROM**                  | Enable / Control Boot ROM Output                        |
| **`C1` – `C3`**    | **RAM**                  | Read, Write, and Memory Enable signals                  |
| **`C4`**           | **IR**                   | Instruction Register Load signal                        |
| **`C5`**           | **Step Counter**         | Instruction Cycle Counter Reset / Step Control          |
| **`C6` – `C8`**    | **IP (PC)**              | Instruction Pointer Increment, Load, and Enable signals |
| **`C9` – `C10`**   | **MAR**                  | Memory Address Register Control                         |
| **`C11`**          | **FLAGS**                | Update Status Flags (`Z`, `S`, `C`, `O`)                |
| **`C12`**          | _Reserved_               | Unassigned / Expansion Pin                              |
| **`C13` – `C15`**  | **High Registers**       | Bus Control for High-byte / Primary Registers           |
| **`C16` – `C18`**  | **Low Registers**        | Bus Control for Low-byte / Secondary Registers          |
| **`C19` – `C31`**  | _Reserved_               | Unassigned Control Lines                                |
| **`C32` – `C35`**  | **ALU Select**           | 4-bit ALU Function Code Selector                        |
| **`C36` – `C55`**  | _Reserved_               | Unassigned Control Lines                                |
| **`C56` – `C59`**  | **Select High Register** | Target High Register Selection (Destination - 4 Bits)   |
| **`C60` – `C63`**  | **Select Low Register**  | Target Low Register Selection (Source - 4 Bits)         |

---

## 🗄️ 2. Register Encodings (4-Bit Register ID)

General-purpose and special registers are encoded using a 4-bit field inside instruction words:

| Binary Code | Register Name | Role / Description   |
| :---------: | :-----------: | :------------------- |
|   `0000`    |   **`AX`**    | Accumulator Register |
|   `0001`    |   **`BX`**    | Base Register        |
|   `0010`    |   **`CX`**    | Counter Register     |
|   `0011`    |   **`DX`**    | Data Register        |
|   `0100`    |   **`BP`**    | Base Pointer         |
|   `0101`    |   **`DI`**    | Destination Index    |
|   `0110`    |   **`SI`**    | Source Index         |
|   `0111`    |   **`SP`**    | Stack Pointer        |
|   `1111`    |  **`FLAG`**   | Flags Register       |

---

## 🏗️ 3. Instruction Word Layout (16-Bit Datapath)

Every instruction word consists of exactly 16 bits, structured as follows:

    [ Bits 15-14 ]  [ Bits 13-10 ]  [ Bits 9-6 ]   [ Bits 5-0 ]
       Reserved        SRC Field     DST Field     Opcode Field
       (2 bits)        (4 bits)       (4 bits)       (6 bits)

- Total Word Size: 2 + 4 + 4 + 6 = **16 Bits**
- Unused fields in single-operand or zero-operand instructions default to `xxxx` (`0000`).

---

## 📜 4. Implemented Instruction Encodings & Bit Patterns

### 1. `BOOT` — System Initialization

Initializes processing pipeline from Boot ROM memory.

    Binary Pattern: [ xx xxxx xxxx 000000 ]
    Opcode: 0x00 (0b000000)

---

### 2. `LOAD` — Load Register from Memory

Reads data from RAM memory address into target destination register (`DST`).

    Binary Pattern: [ xx xxxx [DST] 000001 ]
    Opcode: 0x01 (0b000001)

    [DST] Field (4 Bits):
      0000 -> AX    0100 -> BP
      0001 -> BX    0101 -> DI
      0010 -> CX    0110 -> SI
      0011 -> DX    0111 -> SP
                    1111 -> FLAG

---

### 3. `STOR` — Store Register to Memory

Writes contents of source register (`SRC`) into RAM memory.

    Binary Pattern: [ xx [SRC] xxxx 000010 ]
    Opcode: 0x02 (0b000010)

    [SRC] Field (4 Bits):
      0000 -> AX    0100 -> BP
      0001 -> BX    0101 -> DI
      0010 -> CX    0110 -> SI
      0011 -> DX    0111 -> SP
                    1111 -> FLAG

---

### 4. `MOV` — Register to Register Transfer

Copies content from source register (`SRC`) to destination register (`DST`).

    Binary Pattern: [ xx [SRC] [DST] 000011 ]
    Opcode: 0x03 (0b000011)

    [DST] Target Field (4 Bits) | [SRC] Source Field (4 Bits):
      0000 -> AX                  0000 -> AX
      0001 -> BX                  0001 -> BX
      0010 -> CX                  0010 -> CX
      0011 -> DX                  0011 -> DX
      0100 -> BP                  0100 -> BP
      0101 -> DI                  0101 -> DI
      0110 -> SI                  0110 -> SI
      0111 -> SP                  0111 -> SP
      1111 -> FLAG                1111 -> FLAG

---

### 5. `MOVI` — Move Immediate Value

Loads next 16-bit immediate word directly into target destination register (`DST`).

    Binary Pattern: [ xx xxxx [DST] 000100 ]
    Opcode: 0x04 (0b000100)

    [DST] Field (4 Bits):
      (Same 4-bit Register Encodings)

---

### 6. `ADD` — Register-to-Register Addition

Adds source register value to destination register (`DST <- DST + SRC`).

    Binary Pattern: [ xx [SRC] [DST] 000101 ]
    Opcode: 0x05 (0b000101)

    [DST] Target Field (4 Bits) | [SRC] Source Field (4 Bits):
      (Same 4-bit Register Encodings)

---

### 7. `ADDI` — Immediate Addition

Adds immediate word value directly to destination register (`DST <- DST + IMM`).

    Binary Pattern: [ xx xxxx [DST] 000110 ]
    Opcode: 0x06 (0b000110)

    [DST] Field (4 Bits):
      (Same 4-bit Register Encodings)

---

### 8. `ADDM` — Memory Direct Addition

Adds value at RAM memory location directly to destination register (`DST <- DST + RAM[ADDR]`).

    Binary Pattern: [ xx xxxx [DST] 000111 ]
    Opcode: 0x07 (0b000111)

    [DST] Field (4 Bits):
      (Same 4-bit Register Encodings)

---

### 9. `SUB` — Register-to-Register Subtraction

Subtracts source register value from destination register (`DST <- DST - SRC`).

    Binary Pattern: [ xx [SRC] [DST] 001000 ]
    Opcode: 0x08 (0b001000)

    [DST] Target Field (4 Bits) | [SRC] Source Field (4 Bits):
      (Same 4-bit Register Encodings)

---

### 10. `SUBI` — Immediate Subtraction

Subtracts immediate word value from destination register (`DST <- DST - IMM`).

    Binary Pattern: [ xx xxxx [DST] 001001 ]
    Opcode: 0x09 (0b001001)

    [DST] Field (4 Bits):
      (Same 4-bit Register Encodings)

---

### 11. `SUBM` — Memory Direct Subtraction

Subtracts value at RAM memory location directly from destination register (`DST <- DST - RAM[ADDR]`).

    Binary Pattern: [ xx xxxx [DST] 001010 ]
    Opcode: 0x0A (0b001010)

    [DST] Field (4 Bits):
      (Same 4-bit Register Encodings)
