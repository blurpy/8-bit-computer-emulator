LDA 14     ; Put the value from memory location 14 in the A-register
ADD 15     ; Put the value from memory location 15 in the B-register, and store A+B in the A-register
OUT        ; Output the value of the A-register
HLT        ; Halt the computer
ORG 14     ; Change memory location to 14
DB  28     ; Define a byte with the value 28 at memory location 14
DB  14     ; Define a byte with the value 14 at memory location 15
