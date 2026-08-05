// Test 3: Subroutine Call and Stack Return
MOVI SP, 0x01FF    // Initialize Stack Pointer register
MOVI R0, 0x0020
PUSH R0            // Save initial value onto stack

CALL MULTIPLY_BY_2 // Call subroutine at target label

POP R1             // Restore original pushed value into R1
CMPI R0, 0x0040    // Verify doubled output value (0x0020 * 2 = 0x0040)
JNE FAIL_STACK
HALT               // Pass: Execution completes successfully

MULTIPLY_BY_2:
ADD R0, R0         // Double value in R0
RET                // Pop return PC from stack and return

FAIL_STACK:
MOVI R6, 0x1FFF    // Set failure flag
HALT