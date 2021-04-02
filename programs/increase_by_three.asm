LDI  3     ; Put the value 3 in the A-register
STA 15     ; Store the value from the A-register in memory location 15
LDI  0     ; Put the value 0 in the A-register
ADD 15     ; Put the value from memory location 15 in the B-register, and store A+B in the A-register
OUT        ; Output the value of the A-register
JMP  3     ; Jump to instruction 3
