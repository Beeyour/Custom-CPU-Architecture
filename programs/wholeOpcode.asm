// ============================================================================
// COMPREHENSIVE DIAGNOSTIC SUITE v3 (Memory-Safe)
// R6 stores the current Opcode being tested.
// R5 = 0x0001 (Success) | R5 = 0xBAD0 (Failure)
// ============================================================================

// ----------------------------------------------------------------------------
// 1. DATA MOVEMENT GROUP (0x01 - 0x09)
// ----------------------------------------------------------------------------

TEST_MOVI:
MOVI R6, 0x0006     // Opcode 0x06: MOVI
MOVI R0, 0x1234
CMPI R0, 0x1234
JNE FAIL

TEST_MOV:
MOVI R6, 0x0005     // Opcode 0x05: MOV
MOV R1, R0
CMP R1, R0
JNE FAIL

TEST_STOR_LOAD:
MOVI R6, 0x0002     // Opcode 0x02: STOR
STOR [0x0800], R0   // Write to safe memory space
MOVI R6, 0x0001     // Opcode 0x01: LOAD
LOAD R2, [0x0800]
CMP R2, R0
JNE FAIL

TEST_STR_LDR:
MOVI R3, 0x0810     // Safe base address
MOVI R6, 0x0004     // Opcode 0x04: STR
STR [R3 + 4], R0
MOVI R6, 0x0003     // Opcode 0x03: LDR
LDR R4, [R3 + 4]
CMP R4, R0
JNE FAIL

TEST_PUSH_POP:
MOVI SP, 0x01FF
MOVI R6, 0x0007     // Opcode 0x07: PUSH
PUSH R0
MOVI R6, 0x0008     // Opcode 0x08: POP
POP R5
CMP R5, R0
JNE FAIL

TEST_XCHG:
MOVI R6, 0x0009     // Opcode 0x09: XCHG
MOVI R0, 0xAAAA
MOVI R1, 0x5555
XCHG R0, R1
CMPI R0, 0x5555
JNE FAIL

// ----------------------------------------------------------------------------
// 2. ALU ARITHMETIC GROUP (0x0C - 0x17)
// ----------------------------------------------------------------------------

TEST_ADD:
MOVI R6, 0x000C     // Opcode 0x0C: ADD
MOVI R0, 10
MOVI R1, 5
ADD R0, R1          // 10 + 5 = 15
CMPI R0, 15
JNE FAIL

TEST_ADDI:
MOVI R6, 0x000D     // Opcode 0x0D: ADDI
ADDI R0, 5          // 15 + 5 = 20
CMPI R0, 20
JNE FAIL

TEST_ADDM:
MOVI R6, 0x000E     // Opcode 0x0E: ADDM
STOR [0x0820], R1   // Safe RAM address
ADDM R0, [0x0820]   // 20 + 5 = 25
CMPI R0, 25
JNE FAIL

TEST_SUB:
MOVI R6, 0x000F     // Opcode 0x0F: SUB
SUB R0, R1          // 25 - 5 = 20
CMPI R0, 20
JNE FAIL

TEST_SUBI:
MOVI R6, 0x0010     // Opcode 0x10: SUBI
SUBI R0, 5          // 20 - 5 = 15
CMPI R0, 15
JNE FAIL

TEST_SUBM:
MOVI R6, 0x0011     // Opcode 0x11: SUBM
SUBM R0, [0x0820]   // 15 - 5 = 10
CMPI R0, 10
JNE FAIL

TEST_MUL:
MOVI R6, 0x0012     // Opcode 0x12: MUL
MOVI R0, 6
MOVI R1, 7
MUL R0, R1          // 6 * 7 = 42
CMPI R0, 42
JNE FAIL

TEST_MULI:
MOVI R6, 0x0013     // Opcode 0x13: MULI
MULI R0, 2          // 42 * 2 = 84
CMPI R0, 84
JNE FAIL

TEST_DIV:
MOVI R6, 0x0014     // Opcode 0x14: DIV
MOVI R0, 84
MOVI R1, 2
DIV R0, R1          // 84 / 2 = 42
CMPI R0, 42
JNE FAIL

TEST_DIVI:
MOVI R6, 0x0015     // Opcode 0x15: DIVI
DIVI R0, 7          // 42 / 7 = 6
CMPI R0, 6
JNE FAIL

TEST_INC:
MOVI R6, 0x0016     // Opcode 0x16: INC
INC R0              // 6 + 1 = 7
CMPI R0, 7
JNE FAIL

TEST_DEC:
MOVI R6, 0x0017     // Opcode 0x17: DEC
DEC R0              // 7 - 1 = 6
CMPI R0, 6
JNE FAIL

// ----------------------------------------------------------------------------
// 3. ALU LOGIC, SHIFT & ROTATE GROUP (0x18 - 0x2A)
// ----------------------------------------------------------------------------

TEST_ANDD:
MOVI R6, 0x0018     // Opcode 0x18: ANDD
MOVI R0, 0x0123
MOVI R1, 0x00F0
ANDD R0, R1         // R0 = 0x0020
CMPI R0, 0x0020
JNE FAIL

TEST_ANDI:
MOVI R6, 0x0019     // Opcode 0x19: ANDI
ANDI R0, 0x00F0     // R0 = 0x0020
CMPI R0, 0x0020
JNE FAIL

TEST_ORR:
MOVI R6, 0x001A     // Opcode 0x1A: ORR
MOVI R1, 0x0F00
ORR R0, R1          // R0 = 0x0F20
CMPI R0, 0x0F20
JNE FAIL

TEST_ORI:
MOVI R6, 0x001B     // Opcode 0x1B: ORI
ORI R0, 0x000F      // R0 = 0x0F2F
CMPI R0, 0x0F2F
JNE FAIL

TEST_XORR:
MOVI R6, 0x001C     // Opcode 0x1C: XORR
MOVI R1, 0x0F00
XORR R0, R1         // R0 = 0x002F
CMPI R0, 0x002F
JNE FAIL

TEST_XORI:
MOVI R6, 0x001D     // Opcode 0x1D: XORI
XORI R0, 0x0020     // R0 = 0x000F
CMPI R0, 0x000F
JNE FAIL

TEST_NOTT:
MOVI R6, 0x001E     // Opcode 0x1E: NOTT
MOVI R0, 0x0F0F
NOTT R0             // R0 = 0xF0F0
CMPI R0, 0xF0F0
JNE FAIL

TEST_SHL:
MOVI R6, 0x001F     // Opcode 0x1F: SHL
MOVI R0, 0x0001
MOVI R1, 4
SHL R0, R1          // R0 = 0x0010
CMPI R0, 0x0010
JNE FAIL

TEST_SHLI:
MOVI R6, 0x0020     // Opcode 0x20: SHLI
SHLI R0, 4          // R0 = 0x0100
CMPI R0, 0x0100
JNE FAIL

TEST_SHR:
MOVI R6, 0x0021     // Opcode 0x21: SHR
SHR R0, R1          // R0 = 0x0010
CMPI R0, 0x0010
JNE FAIL

TEST_SHRI:
MOVI R6, 0x0022     // Opcode 0x22: SHRI
SHRI R0, 4          // R0 = 0x0001
CMPI R0, 0x0001
JNE FAIL

TEST_ROLL:
MOVI R6, 0x0023     // Opcode 0x23: ROLL
MOVI R0, 0xF000
MOVI R1, 4
ROLL R0, R1         // R0 = 0x000F
CMPI R0, 0x000F
JNE FAIL

TEST_ROLI:
MOVI R6, 0x0024     // Opcode 0x24: ROLI
ROLI R0, 4          // R0 = 0x00F0
CMPI R0, 0x00F0
JNE FAIL

TEST_RORR:
MOVI R6, 0x0025     // Opcode 0x25: RORR
RORR R0, R1         // R0 = 0x000F
CMPI R0, 0x000F
JNE FAIL

TEST_RORI:
MOVI R6, 0x0026     // Opcode 0x26: RORI
RORI R0, 4          // R0 = 0xF000
CMPI R0, 0xF000
JNE FAIL

TEST_CMP:
MOVI R6, 0x0027     // Opcode 0x27: CMP
MOVI R0, 10
MOVI R1, 10
CMP R0, R1
JNE FAIL

TEST_CMPI:
MOVI R6, 0x0028     // Opcode 0x28: CMPI
CMPI R0, 10
JNE FAIL

TEST_TEST:
MOVI R6, 0x0029     // Opcode 0x29: TEST
MOVI R0, 0x00FF     // 0000 0000 1111 1111
MOVI R1, 0x00F0     // 0000 0000 1111 0000
TEST R0, R1         // Result = 0x00F0 (Not zero)
JZ FAIL             // Will NOT jump because Z = 0

TEST_TESTI:
MOVI R6, 0x002A     // Opcode 0x2A: TESTI
MOVI R0, 0x00FF
TESTI R0, 0x000F    // Result = 0x000F (Not zero)
JZ FAIL             // Will NOT jump because Z = 0
// ----------------------------------------------------------------------------
// 4. CONTROL FLOW GROUP (0x2B - 0x36)
// ----------------------------------------------------------------------------

TEST_JMP:
MOVI R6, 0x002B     // Opcode 0x2B: JMP imm
JMP STEP_JMP_REG
JMP FAIL

STEP_JMP_REG:
MOVI R6, 0x002C     // Opcode 0x2C: JMP reg
MOVI R0, STEP_JZ
JMP R0
JMP FAIL

STEP_JZ:
MOVI R6, 0x002D     // Opcode 0x2D: JZ
MOVI R0, 0
CMPI R0, 0
JZ STEP_JNZ
JMP FAIL

STEP_JNZ:
MOVI R6, 0x002E     // Opcode 0x2E: JNZ
MOVI R0, 5
CMPI R0, 0
JNZ STEP_JC
JMP FAIL

STEP_JC:
MOVI R6, 0x002F     // Opcode 0x2F: JC
STC
JC STEP_JNC
JMP FAIL

STEP_JNC:
MOVI R6, 0x0030     // Opcode 0x30: JNC
CLC
JNC STEP_JG
JMP FAIL

STEP_JG:
MOVI R6, 0x0031     // Opcode 0x31: JG
MOVI R0, 10
MOVI R1, 5
CMP R0, R1
JG STEP_JL
JMP FAIL

STEP_JL:
MOVI R6, 0x0032     // Opcode 0x32: JL
CMP R1, R0
JL STEP_CALL_IMM
JMP FAIL

STEP_CALL_IMM:
MOVI R6, 0x0033     // Opcode 0x33: CALL imm
CALL SUB_IMM
MOVI R6, 0x0034     // Opcode 0x34: CALL reg
MOVI R0, SUB_REG
CALL R0
JMP STEP_LOOP

SUB_IMM:
MOVI R6, 0x0035     // Opcode 0x35: RET
RET

SUB_REG:
RET

STEP_LOOP:
MOVI R6, 0x0036     // Opcode 0x36: LOOP
MOVI R0, 0
MOVI R2, 3
LOOP_BODY:
INC R0
LOOP R2, LOOP_BODY
CMPI R0, 3
JNE FAIL

// ----------------------------------------------------------------------------
// 5. FLAGS & SYSTEM GROUP (0x3C - 0x3F)
// ----------------------------------------------------------------------------

TEST_CLC:
MOVI R6, 0x003C     // Opcode 0x3C: CLC
CLC

TEST_STC:
MOVI R6, 0x003D     // Opcode 0x3D: STC
STC

TEST_CLR:
MOVI R6, 0x003E     // Opcode 0x3E: CLR
MOVI R0, 0x1234
CLR R0
CMPI R0, 0
JNE FAIL

// ----------------------------------------------------------------------------
// ALL TESTS PASSED SUCCESSFULLY
// ----------------------------------------------------------------------------
SUCCESS:
MOVI R5, 0x0001     // R5 = 1 indicates ALL TESTS PASSED!
MOVI R6, 0x0000     // Clear R6 (No errors)
HALT                // Opcode 0x3F: HALT

FAIL:
MOVI R5, 0xBAD0     // R5 = 0xBAD0 indicates TEST FAILURE
HALT                // Check R6 in Logisim to see the failed Opcode!