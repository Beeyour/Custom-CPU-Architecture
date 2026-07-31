# 16-Bit Processor Instruction Set & Control Signal Specification

This document details the hardware control signals, register bit encodings, and binary word layouts for the 16-bit custom processor architecture.

---

## 🎛️ 1. Control Signal Line Mappings (C0 – C63)

The Control Unit outputs a 64-bit control line array (`C0` to `C63`) to orchestrate data movement across internal buses during execution steps.

| Control Line Range | Target Module / Function | Description                                             |
| :----------------- | :----------------------- | :------------------------------------------------------ |
| **`C0`**           | **ROM**                  | Enable / Control Boot ROM Output                        |
| **`C1` – `C4`**    | **RAM**                  | Read, Write, and Memory Enable signals                  |
| **`C5`**           | **IR**                   | Instruction Register Load signal                        |
| **`C6`**           | **Step Counter**         | Instruction Cycle Counter Reset / Step Control          |
| **`C7` – `C9`**    | **IP (PC)**              | Instruction Pointer Increment, Load, and Enable signals |
| **`C10` – `C11`**  | **MAR**                  | Memory Address Register Control                         |
| **`C12`**          | **FLAGS**                | Update Status Flags (`Z`, `S`, `C`, `O`)                |
| **`C13` – `C17`**  | **High Registers**       | Bus Control for High-byte / Primary Registers           |
| **`C18` – `C22`**  | **Low Registers**        | Bus Control for Low-byte / Secondary Registers          |
| **`C23`**          | **Select High ALU**      | Select High ALU Output / Routing Control                |
| **`C24`**          | **Select Low ALU**       | Select Low ALU Output / Routing Control                 |
| **`C25`**          | **ALU1 = 1**             | Set ALU Input 1 Constant (`alu1 = 1`)                   |
| **`C26`**          | **ALU2 = 1**             | Set ALU Input 2 Constant (`alu2 = 1`)                   |
| **`C27` – `C31`**  | _Reserved_               | Unassigned Control Lines                                |
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
       Reserved        SRC / IMM4      DST Field     Opcode Field
       (2 bits)        (4 bits)       (4 bits)       (6 bits)

- Total Word Size: 2 + 4 + 4 + 6 = **16 Bits**
- Unused fields in single-operand or zero-operand instructions default to `xxxx` (`0000`).

---

## 📌 4. Operand & Syntax Notation Guide

This guide defines the standardized placeholders used across instruction syntax definitions:

| Symbol                |  Width  | Description                                             | Assembly Example & Behavior                                     |
| :-------------------- | :-----: | :------------------------------------------------------ | :-------------------------------------------------------------- |
| **`<DST>`**           | 4 Bits  | Destination register ID for results.                    | `MOV AX, BX`<br>_(Copies value of `BX` into `AX`)_              |
| **`<SRC>`**           | 4 Bits  | Source register ID holding input data.                  | `ADD AX, CX`<br>_(Adds `CX` to `AX`, stores in `AX`)_           |
| **`<REG>`**           | 4 Bits  | Target register for single-reg / loop ops.              | `LOOP CX, START`<br>_(Decrements `CX` & branches if `CX != 0`)_ |
| **`[<ADDR>]`**        | 16 Bits | Absolute RAM address. `[ ]` forces RAM access.          | `LOAD AX, [32F1h]`<br>_(Reads RAM at `0x32F1` into `AX`)_       |
| **`[<REG> + <IMM>]`** |   Var   | Base Register + Offset Memory addressing.               | `LAD AX, [CX + 5]` / `STR [BX + 3], CX`                         |
| **`<IMM16>`**         | 16 Bits | 16-bit literal loaded from 2nd instruction word.        | `MOVI AX, 00FFh`<br>_(Loads constant `0x00FF` into `AX`)_       |
| **`<IMM4>`**          | 4 Bits  | 4-bit literal (`0–15` / `0h–Fh`) packed in `SRC` field. | `SHLI AX, Fh`<br>_(Shifts `AX` left by 15 (`0xF`) bits)_        |
| **`<LABEL>`**         |   Var   | Code label resolved to jump address by assembler.       | `JZ PROCESS`<br>_(Jumps to `PROCESS` if Zero `Z = 1`)_          |

---

### 💡 Key Syntax Rules:

- **Memory Brackets `[ ]`:** Plain terms mean registers/constants (`AX`, `32F1h`). Brackets `[32F1h]` trigger a RAM read/write.
- **`<IMM16>` vs `<IMM4>`:** `<IMM16>` requires two memory reads, while `<IMM4>` is embedded directly inside the opcode word.
- **Operand Order:** Target always comes first: `INSTRUCTION <DST>, <SRC>`.

---

## 📜 5. Complete Instruction Encodings & Bit Patterns

### Group 1: Data Movement, Stack & I/O (`0x00` – `0x0B`)

#### 1. `BOOT` / `NOP` — System Initialization & No Operation

    Binary Pattern: [ xx xxxx xxxx 000000 ]
    Opcode: 0x00 (0b000000)
    Syntax: NOP

#### 2. `LOAD` — Load Register Direct from RAM

    Binary Pattern: [ xx xxxx [DST] 000001 ]
    Opcode: 0x01 (0b000001)
    Syntax: LOAD <DST>, [<ADDR>]

#### 3. `STOR` — Store Register Direct to RAM

    Binary Pattern: [ xx [SRC] xxxx 000010 ]
    Opcode: 0x02 (0b000010)
    Syntax: STOR [<ADDR>], <SRC>

#### 4. `LAD` — Load Register with Base + Offset Addressing

    Binary Pattern: [ xx [SRC] [DST] 000011 ]
    Opcode: 0x03 (0b000011)
    Syntax: LAD <DST>, [<REG> + <OFFSET>]

#### 5. `STR` — Store Register with Base + Offset Addressing

    Binary Pattern: [ xx [SRC] [DST] 000100 ]
    Opcode: 0x04 (0b000100)
    Syntax: STR [<REG> + <OFFSET>], <SRC>

#### 6. `MOV` — Register to Register Transfer

    Binary Pattern: [ xx [SRC] [DST] 000101 ]
    Opcode: 0x05 (0b000101)
    Syntax: MOV <DST>, <SRC>

#### 7. `MOVI` — Load Immediate 16-bit Value

    Binary Pattern: [ xx 0111 [DST] 000110 ]
    Opcode: 0x06 (0b000110)
    Syntax: MOVI <DST>, <IMM16>

#### 8. `PUSH` — Push Value onto Stack

    Binary Pattern: [ xx [SRC] 0111 000111 ]
    Opcode: 0x07 (0b000111)
    Syntax: PUSH <SRC>

#### 9. `POP` — Pop Value from Stack

    Binary Pattern: [ xx 0111 [DST] 001000 ]
    Opcode: 0x08 (0b001000)
    Syntax: POP <DST>

#### 10. `XCHG` — Exchange Register Contents

    Binary Pattern: [ xx [SRC] [DST] 001001 ]
    Opcode: 0x09 (0b001001)
    Syntax: XCHG <DST>, <SRC>

#### 11. `INN` — Read from Input Port

    Binary Pattern: [ xx xxxx [DST] 001010 ]
    Opcode: 0x0A (0b001010)
    Syntax: INN <DST>

#### 12. `OUTT` — Write to Output Port

    Binary Pattern: [ xx [SRC] xxxx 001011 ]
    Opcode: 0x0B (0b001011)
    Syntax: OUTT <SRC>

---

### Group 2: ALU & Comparison Operations (`0x0C` – `0x29`)

#### 13. `ADD` — Register-to-Register Addition

    Binary Pattern: [ xx [SRC] [DST] 001100 ]
    Opcode: 0x0C (0b001100)
    Syntax: ADD <DST>, <SRC>

#### 14. `ADDI` — Immediate Addition

    Binary Pattern: [ xx xxxx [DST] 001101 ]
    Opcode: 0x0D (0b001101)
    Syntax: ADDI <DST>, <IMM16>

#### 15. `ADDM` — RAM Direct Addition

    Binary Pattern: [ xx xxxx [DST] 001110 ]
    Opcode: 0x0E (0b001110)
    Syntax: ADDM <DST>, [<ADDR>]

#### 16. `SUB` — Register-to-Register Subtraction

    Binary Pattern: [ xx [SRC] [DST] 001111 ]
    Opcode: 0x0F (0b001111)
    Syntax: SUB <DST>, <SRC>

#### 17. `SUBI` — Immediate Subtraction

    Binary Pattern: [ xx xxxx [DST] 010000 ]
    Opcode: 0x10 (0b010000)
    Syntax: SUBI <DST>, <IMM16>

#### 18. `SUBM` — RAM Direct Subtraction

    Binary Pattern: [ xx xxxx [DST] 010001 ]
    Opcode: 0x11 (0b010001)
    Syntax: SUBM <DST>, [<ADDR>]

#### 19. `MUL` — Unsigned Multiplication (Register)

    Binary Pattern: [ xx [SRC] [DST] 010010 ]
    Opcode: 0x12 (0b010010)
    Syntax: MUL <DST>, <SRC>

#### 20. `MULI` — Immediate Unsigned Multiplication

    Binary Pattern: [ xx xxxx [DST] 010011 ]
    Opcode: 0x13 (0b010011)
    Syntax: MULI <DST>, <IMM16>

#### 21. `DIV` — Unsigned Division (Register)

    Binary Pattern: [ xx [SRC] [DST] 010100 ]
    Opcode: 0x14 (0b010100)
    Syntax: DIV <DST>, <SRC>

#### 22. `DIVI` — Immediate Unsigned Division

    Binary Pattern: [ xx xxxx [DST] 010101 ]
    Opcode: 0x15 (0b010101)
    Syntax: DIVI <DST>, <IMM16>

#### 23. `INC` — Increment Register by 1

    Binary Pattern: [ xx xxxx [DST] 010110 ]
    Opcode: 0x16 (0b010110)
    Syntax: INC <DST>

#### 24. `DEC` — Decrement Register by 1

    Binary Pattern: [ xx xxxx [DST] 010111 ]
    Opcode: 0x17 (0b010111)
    Syntax: DEC <DST>

#### 25. `ANDD` — Bitwise AND Registers

    Binary Pattern: [ xx [SRC] [DST] 011000 ]
    Opcode: 0x18 (0b011000)
    Syntax: ANDD <DST>, <SRC>

#### 26. `ANDI` — Bitwise AND Immediate

    Binary Pattern: [ xx xxxx [DST] 011001 ]
    Opcode: 0x19 (0b011001)
    Syntax: ANDI <DST>, <IMM16>

#### 27. `ORR` — Bitwise OR Registers

    Binary Pattern: [ xx [SRC] [DST] 011010 ]
    Opcode: 0x1A (0b011010)
    Syntax: ORR <DST>, <SRC>

#### 28. `ORI` — Bitwise OR Immediate

    Binary Pattern: [ xx xxxx [DST] 011011 ]
    Opcode: 0x1B (0b011011)
    Syntax: ORI <DST>, <IMM16>

#### 29. `XORR` — Bitwise XOR Registers

    Binary Pattern: [ xx [SRC] [DST] 011100 ]
    Opcode: 0x1C (0b011100)
    Syntax: XORR <DST>, <SRC>

#### 30. `XORI` — Bitwise XOR Immediate

    Binary Pattern: [ xx xxxx [DST] 011101 ]
    Opcode: 0x1D (0b011101)
    Syntax: XORI <DST>, <IMM16>

#### 31. `NOTT` — Bitwise NOT

    Binary Pattern: [ xx xxxx [DST] 011110 ]
    Opcode: 0x1E (0b011110)
    Syntax: NOTT <DST>

#### 32. `SHL` — Logical Shift Left (Register)

    Binary Pattern: [ xx [SRC] [DST] 011111 ]
    Opcode: 0x1F (0b011111)
    Syntax: SHL <DST>, <SRC>

#### 33. `SHLI` — Logical Shift Left (Immediate 4-bit)

    Binary Pattern: [ xx [IMM4] [DST] 100000 ]
    Opcode: 0x20 (0b100000)
    Syntax: SHLI <DST>, <IMM4>

#### 34. `SHR` — Logical Shift Right (Register)

    Binary Pattern: [ xx [SRC] [DST] 100001 ]
    Opcode: 0x21 (0b100001)
    Syntax: SHR <DST>, <SRC>

#### 35. `SHRI` — Logical Shift Right (Immediate 4-bit)

    Binary Pattern: [ xx [IMM4] [DST] 100010 ]
    Opcode: 0x22 (0b100010)
    Syntax: SHRI <DST>, <IMM4>

#### 36. `ROLL` — Rotate Left (Register)

    Binary Pattern: [ xx [SRC] [DST] 100011 ]
    Opcode: 0x23 (0b100011)
    Syntax: ROLL <DST>, <SRC>

#### 37. `ROLI` — Rotate Left (Immediate 4-bit)

    Binary Pattern: [ xx [IMM4] [DST] 100100 ]
    Opcode: 0x24 (0b100100)
    Syntax: ROLI <DST>, <IMM4>

#### 38. `RORR` — Rotate Right (Register)

    Binary Pattern: [ xx [SRC] [DST] 100101 ]
    Opcode: 0x25 (0b100101)
    Syntax: RORR <DST>, <SRC>

#### 39. `RORI` — Rotate Right (Immediate 4-bit)

    Binary Pattern: [ xx [IMM4] [DST] 100110 ]
    Opcode: 0x26 (0b100110)
    Syntax: RORI <DST>, <IMM4>

#### 40. `CMP` — Compare Registers

    Binary Pattern: [ xx [SRC] [DST] 100111 ]
    Opcode: 0x27 (0b100111)
    Syntax: CMP <DST>, <SRC>

#### 41. `CMPI` — Compare Immediate

    Binary Pattern: [ xx xxxx [DST] 101000 ]
    Opcode: 0x28 (0b101000)
    Syntax: CMPI <DST>, <IMM16>

#### 42. `TEST` — Bitwise Logical Compare

    Binary Pattern: [ xx [SRC] [DST] 101001 ]
    Opcode: 0x29 (0b101001)
    Syntax: TEST <DST>, <SRC>

---

### Group 3: Control Flow & Branching (`0x2A` – `0x37`)

#### 43. `JMP` — Unconditional Jump

    Binary Pattern: [ xx xxxx xxxx 101010 ]
    Opcode: 0x2A (0b101010)
    Syntax: JMP <LABEL>

#### 44. `JZ` / `JE` — Jump if Zero (Z = 1)

    Binary Pattern: [ xx xxxx xxxx 101011 ]
    Opcode: 0x2B (0b101011)
    Syntax: JZ <LABEL>  /  JE <LABEL>

#### 45. `JNZ` / `JNE` — Jump if Not Zero (Z = 0)

    Binary Pattern: [ xx xxxx xxxx 101100 ]
    Opcode: 0x2C (0b101100)
    Syntax: JNZ <LABEL>  /  JNE <LABEL>

#### 46. `JC` — Jump if Carry (C = 1)

    Binary Pattern: [ xx xxxx xxxx 101101 ]
    Opcode: 0x2D (0b101101)
    Syntax: JC <LABEL>

#### 47. `JNC` — Jump if Not Carry (C = 0)

    Binary Pattern: [ xx xxxx xxxx 101110 ]
    Opcode: 0x2E (0b101110)
    Syntax: JNC <LABEL>

#### 48. `JS` — Jump if Sign (S = 1)

    Binary Pattern: [ xx xxxx xxxx 101111 ]
    Opcode: 0x2F (0b101111)
    Syntax: JS <LABEL>

#### 49. `JNS` — Jump if Not Sign (S = 0)

    Binary Pattern: [ xx xxxx xxxx 110000 ]
    Opcode: 0x30 (0b110000)
    Syntax: JNS <LABEL>

#### 50. `JA` / `JG` — Jump if Greater / Above

    Binary Pattern: [ xx xxxx xxxx 110001 ]
    Opcode: 0x31 (0b110001)
    Syntax: JA <LABEL>  /  JG <LABEL>

#### 51. `REF` / `JL` — Jump if Less

    Binary Pattern: [ xx xxxx xxxx 110010 ]
    Opcode: 0x32 (0b110010)
    Syntax: REF <LABEL>  /  JL <LABEL>

#### 52. `JAE` — Jump if Greater or Equal

    Binary Pattern: [ xx xxxx xxxx 110011 ]
    Opcode: 0x33 (0b110011)
    Syntax: JAE <LABEL>

#### 53. `JBE` — Jump if Less or Equal

    Binary Pattern: [ xx xxxx xxxx 110100 ]
    Opcode: 0x34 (0b110100)
    Syntax: JBE <LABEL>

#### 54. `CALL` — Call Subroutine

    Binary Pattern: [ xx xxxx xxxx 110101 ]
    Opcode: 0x35 (0b110101)
    Syntax: CALL <LABEL>

#### 55. `RET` — Return from Subroutine

    Binary Pattern: [ xx xxxx xxxx 110110 ]
    Opcode: 0x36 (0b110110)
    Syntax: RET

#### 56. `LOOP` — Decrement Register & Branch if Reg != 0

    Binary Pattern: [ xx xxxx [REG] 110111 ]
    Opcode: 0x37 (0b110111)
    Syntax: LOOP <REG>, <LABEL>

---

### Group 4: Flags & System Operations (`0x38` – `0x3B`)

#### 57. `CLC` — Clear Carry Flag

    Binary Pattern: [ xx xxxx xxxx 111000 ]
    Opcode: 0x38 (0b111000)
    Syntax: CLC

#### 58. `STC` — Set Carry Flag

    Binary Pattern: [ xx xxxx xxxx 111001 ]
    Opcode: 0x39 (0b111001)
    Syntax: STC

#### 59. `CLR` — Clear Register / Control Flags

    Binary Pattern: [ xx xxxx [DST] 111010 ]
    Opcode: 0x3A (0b111010)
    Syntax: CLR <DST>

#### 60. `HALT` — Halt CPU Clock & Pipeline

    Binary Pattern: [ xx xxxx xxxx 111011 ]
    Opcode: 0x3B (0b111011)
    Syntax: HALT
