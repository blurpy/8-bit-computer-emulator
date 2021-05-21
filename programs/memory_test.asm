LDA 14     ; Put the value from memory location 14 in the A-register
ADD 15     ; Put the value from memory location 15 in the B-register, and store A+B in the A-register
STA 14     ; Store the value from the A-register in memory location 14
OUT        ; Output the value of the A-register
HLT        ; Halt the computer
ORG 14     ; Change memory location to 14
DB   3     ; Define a byte with the value 3 at memory location 14
DB   4     ; Define a byte with the value 4 at memory location 15
