; === EXECUTION START ===
LOAD AX, [word1]
LOAD BX, [word2]
STOR [word1 + 1], BX
HALT
; === DATA SECTION ===


word1: dw FFh, 1fh
word2: dw 32, 1010b
