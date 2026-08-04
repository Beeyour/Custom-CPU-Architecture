; === EXECUTION START ===
LOAD R0, [word1]
MOVI R1, 1
STOR [word2], R1
MOVI R2, 0Fh
STR [word2 + R0], R2
LDR R3, [R0 + word2]
MOVI SP, 100h
PUSH R0
PUSH R1
POP R4
POP R5
POP R6
HALT

; === DATA SECTION ===

word1: dw FFh, 1fh
word2: dw 32, 1010b
