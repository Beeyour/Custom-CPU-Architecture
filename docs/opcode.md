# 16-Bit Processor Instruction Set & Control Signal Specification

This document details the hardware control signals, register bit encodings, and binary word layouts for the 16-bit custom processor architecture.

---

## 🎛️ 1. Control Signal Line Mappings (C0 – C63)

The Control Unit outputs a 64-bit control line array (`C0` to `C63`) to orchestrate data movement across internal buses during execution steps.

| Control Line Range            | Target Module / Function | Description                                             |
| :---------------------------- | :----------------------- | :------------------------------------------------------ |
| **`C0`**                      | **ROM**                  | Enable / Control Boot ROM Output                        |
| **`C1` – `C4`**               | **RAM**                  | Read, Write, and Memory Enable signals                  |
| **`C5`**                      | **IR**                   | Instruction Register Load signal                        |
| **`C6`**                      | **Step Counter**         | Instruction Cycle Counter Reset / Step Control          |
| **`C7` – `C9`, `C27`, `C29`** | **IP (PC)**              | Instruction Pointer Increment, Load, and Enable signals |
| **`C10` – `C11`**             | **MAR**                  | Memory Address Register Control                         |
| **`C12`, `C28`**              | **FLAGS**                | Update Status Flags (`Z`, `S`, `C`, `V`)                |
| **`C13` – `C17`**             | **High Registers**       | Bus Control for High-byte / Primary Registers           |
| **`C18` – `C22`**             | **Low Registers**        | Bus Control for Low-byte / Secondary Registers          |
| **`C23`**                     | **Select High ALU**      | Select High ALU Output / Routing Control                |
| **`C24`**                     | **Select Low ALU**       | Select Low ALU Output / Routing Control                 |
| **`C25`**                     | **ALU1 = 1**             | Set ALU Input 1 Constant (`alu1 = 1`)                   |
| **`C26`**                     | **ALU2 = 1**             | Set ALU Input 2 Constant (`alu2 = 1`)                   |
| **`C30` – `C31`**             | _Reserved_               | Unassigned Control Lines                                |
| **`C32` – `C35`**             | **ALU Select**           | 4-bit ALU Function Code Selector                        |
| **`C36` – `C55`**             | _Reserved_               | Unassigned Control Lines                                |
| **`C56` – `C59`**             | **Select High Register** | Target High Register Selection (Destination - 4 Bits)   |
| **`C60` – `C63`**             | **Select Low Register**  | Target Low Register Selection (Source - 4 Bits)         |

---

## 🗄️ 2. Register Encodings (4-Bit Register ID)

General-purpose and special registers are encoded using a 4-bit field inside instruction words:

| Binary Code | Register Name | Role / Description   |
| :---------: | :-----------: | :------------------- |
|   `0000`    |   **`R0`**    | Accumulator Register |
|   `0001`    |   **`R1`**    | Base Register        |
|   `0010`    |   **`R2`**    | Counter Register     |
|   `0011`    |   **`R3`**    | Data Register        |
|   `0100`    |   **`R4`**    | Base Pointer         |
|   `0101`    |   **`R5`**    | Destination Index    |
|   `0110`    |   **`R6`**    | Source Index         |
|   `1110`    |   **`SP`**    | Stack Pointer        |
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
| **`<DST>`**           | 4 Bits  | Destination register ID for results.                    | `MOV R0, R1`<br>_(Copies value of `R1` into `R0`)_              |
| **`<SRC>`**           | 4 Bits  | Source register ID holding input data.                  | `ADD R0, R2`<br>_(Adds `R2` to `R0`, stores in `R0`)_           |
| **`<REG>`**           | 4 Bits  | Target register for single-reg / loop ops.              | `LOOP R2, START`<br>_(Decrements `R2` & branches if `R2 != 0`)_ |
| **`[<ADDR>]`**        | 16 Bits | Absolute RAM address. `[ ]` forces RAM access.          | `LOAD R0, [32F1h]`<br>_(Reads RAM at `0x32F1` into `R0`)_       |
| **`[<REG> + <IMM>]`** |   Var   | Base Register + Offset Memory addressing.               | `LDR R0, [R2 + 5]` / `STR [R1 + 3], R2`                         |
| **`<IMM16>`**         | 16 Bits | 16-bit literal loaded from 2nd instruction word.        | `MOVI R0, 00FFh`<br>_(Loads constant `0x00FF` into `R0`)_       |
| **`<IMM4>`**          | 4 Bits  | 4-bit literal (`0–15` / `0h–Fh`) packed in `SRC` field. | `SHLI R0, Fh`<br>_(Shifts `R0` left by 15 (`0xF`) bits)_        |
| **`<LABEL>`**         |   Var   | Code label resolved to jump address by assembler.       | `JZ PROCESS`<br>_(Jumps to `PROCESS` if Zero `Z = 1`)_          |

---

### 💡 Key Syntax Rules:

- **Memory Brackets `[ ]`:** Plain terms mean registers/constants (`R0`, `32F1h`). Brackets `[32F1h]` trigger a RAM read/write.
- **`<IMM16>` vs `<IMM4>`:** `<IMM16>` requires two memory words (32 bits total), while `<IMM4>` is embedded directly inside the 16-bit opcode word.
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

#### 4. `LDR` — Load Register with Base + Offset Addressing

    Binary Pattern: [ xx [SRC] [DST] 000011 ]
    Opcode: 0x03 (0b000011)
    Syntax: LDR <DST>, [<SRC> + <IMM16>]

#### 5. `STR` — Store Register with Base + Offset Addressing

    Binary Pattern: [ xx [SRC] [DST] 000100 ]
    Opcode: 0x04 (0b000100)
    Syntax: STR [<DST> + <IMM16>], <SRC>

#### 6. `MOV` — Register to Register Transfer

    Binary Pattern: [ xx [SRC] [DST] 000101 ]
    Opcode: 0x05 (0b000101)
    Syntax: MOV <DST>, <SRC>

#### 7. `MOVI` — Load Immediate 16-bit Value

    Binary Pattern: [ xx xxxx [DST] 000110 ]
    Opcode: 0x06 (0b000110)
    Syntax: MOVI <DST>, <IMM16>

#### 8. `PUSH` — Push Value onto Stack

    Binary Pattern: [ xx [SRC] 1110 000111 ]
    Opcode: 0x07 (0b000111)
    Syntax: PUSH <SRC>

#### 9. `POP` — Pop Value from Stack

    Binary Pattern: [ xx 1110 [DST] 001000 ]
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

### Group 2: ALU & Comparison Operations (`0x0C` – `0x2A`)

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

#### 42. `TEST` — Bitwise Logical Compare Registers

    Binary Pattern: [ xx [SRC] [DST] 101001 ]
    Opcode: 0x29 (0b101001)
    Syntax: TEST <DST>, <SRC>

#### 43. `TESTI` — Bitwise Logical Compare Immediate

    Binary Pattern: [ xx xxxx [DST] 101010 ]
    Opcode: 0x2A (0b101010)
    Syntax: TESTI <DST>, <IMM16>

---

### Group 3: Control Flow & Branching (`0x2B` – `0x36`)

#### 44. `JMP imm` (Logisim: `JUMP`) — Unconditional Immediate Jump

    Binary Pattern: [ xx xxxx xxxx 101011 ]
    Opcode: 0x2B (0b101011)
    Syntax: JMP <LABEL>

#### 45. `JMP reg` (Logisim: `JUMP_R`) — Register Indirect Jump

    Binary Pattern: [ xx [SRC] xxxx 101100 ]
    Opcode: 0x2C (0b101100)
    Syntax: JMP <SRC>

#### 46. `JZ` / `JE` — Jump if Zero / Jump if Equal

    Binary Pattern: [ xx xxxx xxxx 101101 ]
    Opcode: 0x2D (0b101101)
    Syntax: JZ <LABEL>  /  JE <LABEL>

#### 47. `JNZ` / `JNE` — Jump if Not Zero / Jump if Not Equal

    Binary Pattern: [ xx xxxx xxxx 101110 ]
    Opcode: 0x2E (0b101110)
    Syntax: JNZ <LABEL>  /  JNE <LABEL>

#### 48. `JC` / `JB` — Jump if Carry / Unsigned Below

    Binary Pattern: [ xx xxxx xxxx 101111 ]
    Opcode: 0x2F (0b101111)
    Syntax: JC <LABEL>  /  JB <LABEL>

#### 49. `JNC` / `JAE` — Jump if No Carry / Unsigned Above or Equal

    Binary Pattern: [ xx xxxx xxxx 110000 ]
    Opcode: 0x30 (0b110000)
    Syntax: JNC <LABEL>  /  JAE <LABEL>

#### 50. `JG` — Jump if Signed Greater

    Binary Pattern: [ xx xxxx xxxx 110001 ]
    Opcode: 0x31 (0b110001)
    Syntax: JG <LABEL>

#### 51. `JL` — Jump if Signed Less

    Binary Pattern: [ xx xxxx xxxx 110010 ]
    Opcode: 0x32 (0b110010)
    Syntax: JL <LABEL>

#### 52. `CALL imm` (Logisim: `CALL`) — Direct Subroutine Call

    Binary Pattern: [ xx xxxx 1110 110011 ]
    Opcode: 0x33 (0b110011)
    Syntax: CALL <LABEL>

#### 53. `CALL reg` (Logisim: `CALL_R`) — Register Indirect Subroutine Call

    Binary Pattern: [ xx [SRC] 1110 110100 ]
    Opcode: 0x34 (0b110100)
    Syntax: CALL <SRC>

#### 54. `RET` — Return from Subroutine

    Binary Pattern: [ xx 1110 xxxx 110101 ]
    Opcode: 0x35 (0b110101)
    Syntax: RET

#### 55. `LOOP` (Logisim: `LOOPP`) — Decrement Register & Branch if Reg != 0

    Binary Pattern: [ xx xxxx [REG] 110110 ]
    Opcode: 0x36 (0b110110)
    Syntax: LOOP <REG>, <LABEL>

---

### Reserved Opcodes (`0x37` – `0x3B`)

    Unallocated 6-bit Opcode Range: 0x37 – 0x3B (0b110111 – 0b111011)
    Purpose: Reserved for future instruction expansion (5 free slots)

---

### Group 4: Flags & System Operations (`0x3C` – `0x3F`)

#### 56. `CLC` — Clear Carry Flag

    Binary Pattern: [ xx xxxx xxxx 111100 ]
    Opcode: 0x3C (0b111100)
    Syntax: CLC

#### 57. `STC` — Set Carry Flag

    Binary Pattern: [ xx xxxx xxxx 111101 ]
    Opcode: 0x3D (0b111101)
    Syntax: STC

#### 58. `CLR` — Clear Register / Control Flags

    Binary Pattern: [ xx xxxx [DST] 111110 ]
    Opcode: 0x3E (0b111110)
    Syntax: CLR <DST>

#### 59. `HALT` — Halt CPU Clock & Pipeline

    Binary Pattern: [ xx xxxx xxxx 111111 ]
    Opcode: 0x3F (0b111111)
    Syntax: HALT
