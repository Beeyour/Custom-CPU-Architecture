// Test Group 4: Manual Flag Updates, Register Clear, and Pipeline Control
CLC                 // Clear Carry Flag (C = 0)
STC                 // Set Carry Flag (C = 1)

MOVI R0, 0x1234
CLR R0              // Clear register contents: R0 = 0
CMPI R0, 0          // Verify R0 is cleared
JNE FAIL_G4

MOVI R1, 1          // Success flag in R1
HALT

FAIL_G4:
MOVI R1, 0xFFFF     // Failure flag in R1
HALT