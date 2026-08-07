// ============================================================================
// Code section
// ============================================================================

mainCode:
    MOVI SP, 0x1000       // Initialize Stack Pointer
    MOVI R1, myArr        // R1 = Pointer to array start
    MOVI R2, 5            // R2 = Element count
    MOVI R0, 0            // R0 = Accumulator sum

sumLoop:
    LDR R3, [R1 + 0]      // Load current element into R3
    ADD R0, R3            // Add element to accumulator
    INC R1                // Advance memory pointer
    LOOP R2, sumLoop      // Decrement R2 and loop if R2 != 0

    CMPI R0, 150          // Expected sum: 10 + 20 + 30 + 40 + 50 = 150 (0x0096)
    JNE FAIL              // Jump to FAIL if sum is incorrect

PASS:
    MOVI R5, 0x0001       // R5 = 0x0001 indicates PASS
    HALT

FAIL:
    MOVI R5, 0xBAD0       // R5 = 0xBAD0 indicates FAIL
    HALT

// ============================================================================
// Data section
// ============================================================================

myArr: dw 10, 20, 30, 40, 50