// ============================================================================
// Code section
// ============================================================================

mainCode:
    MOVI SP, 0x1000       // Initialize Stack Pointer
    MOVI R1, searchData   // R1 = Array start address
    MOVI R2, 5            // R2 = Array size
    MOVI R3, 0x0034       // R3 = Target value to find

    CALL searchSubroutine

    CMPI R0, 2            // Expected index of 0x0034 is 2
    JNE FAIL

PASS:
    MOVI R5, 0x0001       // R5 = 0x0001 indicates PASS
    HALT

FAIL:
    MOVI R5, 0xBAD0       // R5 = 0xBAD0 indicates FAIL
    HALT

// ----------------------------------------------------------------------------
// Search Subroutine: Searches for R3 in array pointed by R1
// Output: R0 = Found Index (or 0xFFFF if not found)
// ----------------------------------------------------------------------------
searchSubroutine:
    PUSH R1               // Save original registers to stack
    PUSH R2
    MOVI R0, 0            // Initialize index counter

searchLoop:
    LDR R4, [R1 + 0]      // Load array element
    CMP R4, R3            // Compare element with target
    JZ foundMatch         // Jump if equal (Z = 1)

    INC R1                // Increment pointer
    INC R0                // Increment index
    LOOP R2, searchLoop   // Decrement R2 and continue

    MOVI R0, 0xFFFF       // Not found flag

foundMatch:
    POP R2                // Restore registers from stack
    POP R1
    RET

// ============================================================================
// Data section
// ============================================================================

searchData: dw 0x0005, 0x0012, 0x0034, 0x0078, 0x00AB