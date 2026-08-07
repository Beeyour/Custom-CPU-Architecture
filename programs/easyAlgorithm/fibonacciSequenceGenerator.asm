// ============================================================================
// Code section
// ============================================================================

mainCode:
    MOVI SP, 0x1000       // Initialize Stack Pointer
    MOVI R1, fibBuffer    // R1 = Base address of Fibonacci storage

    // Store base cases: Fib(0) = 0, Fib(1) = 1
    MOVI R0, 0
    STR [R1 + 0], R0
    MOVI R0, 1
    STR [R1 + 1], R0

    MOVI R2, 5            // Generate 5 more numbers (Fib 2 through 6)

fibLoop:
    LDR R3, [R1 + 0]      // Load Fib(N-2)
    INC R1                // Advance base pointer
    LDR R4, [R1 + 0]      // Load Fib(N-1)
    ADD R4, R3            // Calculate Fib(N) = Fib(N-1) + Fib(N-2)
    STR [R1 + 1], R4      // Store Fib(N) at next position
    LOOP R2, fibLoop

    // Reset pointer and verify Fib(6) equals 8 (Sequence: 0, 1, 1, 2, 3, 5, 8)
    MOVI R1, fibBuffer
    LDR R0, [R1 + 6]      // Read 7th entry (index 6)
    CMPI R0, 8
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

fibBuffer: dw 0, 0, 0, 0, 0, 0, 0