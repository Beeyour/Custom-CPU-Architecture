// Test Group 1: Memory Transfers, Register Copying, Stack, and Exchange
MOVI R0, 0x1234     // Load 0x1234 into R0
MOV R1, R0          // Copy R0 to R1
CMP R0, R1          // Verify R1 == 0x1234
JNE FAIL_G1

STOR [0x0080], R0   // Store R0 to RAM address 0x0080
LOAD R2, [0x0080]   // Load RAM address 0x0080 into R2
CMP R0, R2          // Verify R2 == 0x1234
JNE FAIL_G1

MOVI R3, 0x0100     // Set base address in R3
MOVI R4, 0xABCD     // Set test data in R4
STR [R3 + 4], R4    // Store R4 into RAM [0x0100 + 4] = 0x0104
LDR R5, [R3 + 4]    // Load back into R5
CMP R4, R5          // Verify R5 == 0xABCD
JNE FAIL_G1

MOVI SP, 0x01FF     // Initialize Stack Pointer
PUSH R0             // Push 0x1234 onto stack
POP R6              // Pop from stack into R6
CMP R0, R6          // Verify stack pop
JNE FAIL_G1

MOVI R0, 0xAAAA
MOVI R1, 0x5555
XCHG R0, R1         // Swap contents: R0 = 0x5555, R1 = 0xAAAA
CMPI R0, 0x5555
JNE FAIL_G1

MOVI R0, 1          // Success flag in R0
HALT

FAIL_G1:
MOVI R0, 0xFFFF     // Failure flag in R0
HALT