// ============================================================================
// يوم للتاريخ، تم الانتهاء من اول خوارزميه كتبتها بنفسي بتاريخ ٢٤/٢/١٤٤٨، ٧/٨/٢٠٢٦، وبذلك اعلن انتهاء مشروع المعالج الخاص ، الحمدلله دائما وابدا، قد تكون هناك ميزات اضافية باذن الله بشكل مبسط جدا
// Code section
// ============================================================================


mainCode:
    MOVI SP, 1000h
    MOVI R5, myArr
    MOVI R6, stop
    CALL sortingArray
    HALT

; R5 is the bignning of array
; R6 is the end of array
sortingArray:
    SUB R6, R5
    MOV R4, R6
    DEC R4
    DEC R5
replaceMin:
        INC R5
        MOVI R6, stop
        CALL minmumOfArray
        LDR R1, [R5]
        LDR R2, [R0]
        STR [R5], R2
        STR [R0], R1
        LOOP R4, replaceMin
    RET

; R5 is the bignning of array
; R6 is the end of array
; result min on R0
; current on R5
minmumOfArray:
    MOV R0, R5
    MOV R3, R6
    SUB R6, R5
findMin:
        LDR R1, [R0]
        LDR R2, [R3]
        CMP R1, R2
        JB isLess
        MOV R0, R3
isLess:
        DEC R3
        LOOP R6, findMin
    RET


// ============================================================================
// Data section
// ============================================================================

myArr: dw 52fh, EEF3h, 52h, 62h, 425h, 52fh, 252Eh, 42h
stop: