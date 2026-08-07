// Test Group 2: Arithmetic, Bitwise Logic, Shifts, and Rotations
MOVI R0, 10
ADDI R0, 15         // R0 = 10 + 15 = 25
SUBI R0, 5          // R0 = 25 - 5 = 20
INC R0              // R0 = 20 + 1 = 21
DEC R0              // R0 = 21 - 1 = 20
CMPI R0, 20         // Verify arithmetic result
JNE FAIL_G2

MOVI R1, 0x00FF
ANDI R1, 0x000F     // Bitwise AND: R1 = 0x000F
SHLI R1, 4          // Shift Left by 4: R1 = 0x00F0
ORI R1, 0x0005      // Bitwise OR: R1 = 0x00F5
XORI R1, 0x00F0     // Bitwise XOR: R1 = 0x0005
CMPI R1, 5          // Verify logic and shift results
JNE FAIL_G2

MOVI R2, 0x0F0F
NOTT R2             // Bitwise NOT: R2 = 0xF0F0
ROLI R2, 4          // Rotate Left by 4: R2 = 0x0F0F
CMPI R2, 0x0F0F     // Verify rotation restored value
JNE FAIL_G2

MOVI R0, 1          // Success flag in R0
HALT

FAIL_G2:
MOVI R0, 0xFFFF     // Failure flag in R0
HALT