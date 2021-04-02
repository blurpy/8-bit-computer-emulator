LDA 14     ; Put the value from memory location 14 in the A-register
SUB 12     ; Put the value from memory location 12 in the B-register, and store A-B in the A-register
JC   6     ; Jump to instruction 6 if the A-register is past 255
LDA 13     ; Put the value from memory location 13 in the A-register
OUT        ; Output the value of the A-register
HLT        ; Halt the computer
STA 14     ; Store the value from the A-register in memory location 14
LDA 13     ; Put the value from memory location 13 in the A-register
ADD 15     ; Put the value from memory location 15 in the B-register, and store A+B in the A-register
STA 13     ; Store the value from the A-register in memory location 13
JMP  0     ; Jump to instruction 0
ORG 12     ; Change memory location to 12
DB   1     ; Define a byte with the value 1 at memory location 12
DB   0     ; Define a byte with the value 0 at memory location 13
DB   7     ; Define a byte with the value 7 at memory location 14
DB   8     ; Define a byte with the value 8 at memory location 15
