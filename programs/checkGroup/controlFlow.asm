// Test Group 3: Conditional Jumps, Subroutine Calls, and Loop Counters
MOVI R0, 0
CMPI R0, 0          // 0 - 0 = 0 (Sets Z = 1)
JZ TEST_JNZ         // Jump if Z == 1
HALT                // Fail: JZ did not jump

TEST_JNZ:
MOVI R0, 5
CMPI R0, 2          // 5 - 2 = 3 (Sets Z = 0)
JNZ TEST_COMPARE    // Jump if Z == 0
HALT                // Fail: JNZ did not jump

TEST_COMPARE:
MOVI R1, 20
MOVI R2, 10
CMP R1, R2          // 20 - 10 = 10 (S = 0, Z = 0, V = 0)
JG TEST_CALL        // Jump if S == V and Z == 0
HALT                // Fail: JG did not jump

TEST_CALL:
MOVI SP, 0x01FF     // Initialize Stack Pointer
MOVI R0, 10
CALL MULTIPLY_SUB   // Call subroutine (R0 = 10 * 2 = 20)
CMPI R0, 20
JNE FAIL_G3

MOVI R0, 0          // Accumulator sum
MOVI R2, 5          // Iteration counter
SUM_LOOP:
ADD R0, R2          // Accumulate sum: 5 + 4 + 3 + 2 + 1 = 15
LOOP R2, SUM_LOOP   // Decrement R2 and branch if R2 != 0
CMPI R0, 15
JNE FAIL_G3

MOVI R0, 1          // Set success flag in R0
HALT

MULTIPLY_SUB:
ADD R0, R0          // Double R0
RET                 // Return from subroutine

FAIL_G3:
MOVI R0, 0x1FFF     // Set failure flag in R0
HALT