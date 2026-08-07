// ============================================================================
// Code section
// ============================================================================

mainCode:
    MOVI SP, 0x1000       // Initialize Stack Pointer
    MOVI R1, vecA         // R1 = Pointer to Vector A
    MOVI R2, vecB         // R2 = Pointer to Vector B
    MOVI R6, 3            // R6 = Vector size (3 pairs)
    MOVI R0, 0            // R0 = Dot product sum

dotLoop:
    LDR R3, [R1 + 0]      // Load from Vector A
    LDR R4, [R2 + 0]      // Load from Vector B
    MUL R3, R4            // R3 = R3 * R4
    ADD R0, R3            // R0 = R0 + product

    INC R1                // Move Vector A pointer forward
    INC R2                // Move Vector B pointer forward
    LOOP R6, dotLoop      // Decrement R6 and repeat

    // Expected Result: (2 * 10) + (5 * 20) + (3 * 6) = 20 + 100 + 18 = 138 (0x008A)
    CMPI R0, 138
    JNE FAIL

PASS:
    MOVI R5, 0x0001       // R5 = 0x0001 indicates PASS
    HALT

FAIL:
    MOVI R5, 0xBAD0       // R5 = 0xBAD0 indicates FAIL
    HALT

// ============================================================================
// Data section
// ============================================================================

vecA: dw 2, 5, 3
vecB: dw 10, 20, 6