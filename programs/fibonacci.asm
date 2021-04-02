LDI  0     ; Put the value 0 in the A-register
STA 13     ; Store the value from the A-register in memory location 13
OUT        ; Output the value of the A-register
LDI  1     ; Put the value 1 in the A-register
STA 14     ; Store the value from the A-register in memory location 14
OUT        ; Output the value of the A-register
ADD 13     ; Put the value from memory location 13 in the B-register, and store A+B in the A-register
JC   0     ; Jump to instruction 0 if the A-register is past 255
STA 15     ; Store the value from the A-register in memory location 15
LDA 14     ; Put the value from memory location 14 in the A-register
STA 13     ; Store the value from the A-register in memory location 13
LDA 15     ; Put the value from memory location 15 in the A-register
JMP  4     ; Jump to instruction 4
DB   0     ; Define a byte with the value 0 at memory location 13
DB   0     ; Define a byte with the value 0 at memory location 14
DB   0     ; Define a byte with the value 0 at memory location 15
