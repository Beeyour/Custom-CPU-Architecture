# C0 _ROM_

# C1, C2, C3 _RAM_

# C4 _IR_

# C5 _Counter_

# C6, C7, C8 _IP_

# C9, C10 _MAR_

# C11 _FLAGS_

$ C12 $

# C13,C14, C15 _For High Registers_

# C16, C17, C18 _For Low Registers_

$C19_C31$

# C32, C33, C34, C35 _Select ALU_

$C36_C55$

# C56, C57, C58, C59 _Select High Register_

# C60, C61, C62, C63 _Select High Register_

$

% BOOT_0

xxxx xxxx xx00 0000

% LOAD_1 (LOAD FROM SPECIFIC ADDRESS)
xxxx xxxx xx00 0001

xxxx xx00 00 To AX
xxxx xx00 01 To BX
xxxx xx00 10 To CX
xxxx xx00 11 To DX

% STOR_2 (STORE TO SPECIFIC ADDRESS)
xxxx xxxx xx00 0010

xx00 00 From AX
xx00 01 From BX
xx00 10 From CX
xx00 11 From DX

% MOV_3 (MOV FROM RIGSTER TO RIGSTER)
xxxx xxxx xx00 0011

xxxx xx00 00 To AX
xxxx xx00 01 To BX
xxxx xx00 10 To CX
xxxx xx00 11 To DX

xx00 00 From AX
xx00 01 From BX
xx00 10 From CX
xx00 11 From DX

% MOVI_4 (MOV NEXT NUMBER IMMEDTE TO RIGSTER)
xxxx xxxx xx00 0100

xxxx xx00 00 To AX
xxxx xx00 01 To BX
xxxx xx00 10 To CX
xxxx xx00 11 To DX

% ADD_5 (ADD RIGSTER TO RIGSTER)
xxxx xxxx xx00 0101

xxxx xx00 00 To AX
xxxx xx00 01 To BX
xxxx xx00 10 To CX
xxxx xx00 11 To DX

xx00 00 From AX
xx00 01 From BX
xx00 10 From CX
xx00 11 From DX

% ADDI_6 (ADD NEXT NUMBER IMMEDTE TO RIGSTER)
xxxx xxxx xx00 0110

xxxx xx00 00 To AX
xxxx xx00 10 To CX
xxxx xx00 01 To BX
xxxx xx00 11 To DX

% ADDM_7 (ADD FROM SPECIFIC ADDRESS TO RIGSTER)
xxxx xxxx xx00 0111

xxxx xx00 00 To AX
xxxx xx00 10 To CX
xxxx xx00 01 To BX
xxxx xx00 11 To DX

% SUB_8 (SUB RIGSTER TO RIGSTER)
xxxx xxxx xx00 1000

xxxx xx00 00 To AX
xxxx xx00 01 To BX
xxxx xx00 10 To CX
xxxx xx00 11 To DX

xx00 00 From AX
xx00 01 From BX
xx00 10 From CX
xx00 11 From DX

% SUBI_9 (SUB NEXT NUMBER IMMEDTE TO RIGSTER)
xxxx xxxx xx00 1001

xxxx xx00 00 To AX
xxxx xx00 10 To CX
xxxx xx00 01 To BX
xxxx xx00 11 To DX

% SUBM_10 (SUB FROM SPECIFIC ADDRESS TO RIGSTER)
xxxx xxxx xx00 1010

xxxx xx00 00 To AX
xxxx xx00 10 To CX
xxxx xx00 01 To BX
xxxx xx00 11 To DX
