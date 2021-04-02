LDA 14     ; Put the value from memory location 14 in the A-register
SUB 15     ; Put the value from memory location 15 in the B-register, and store A-B in the A-register
OUT        ; Output the value of the A-register
HLT        ; Halt the computer
ORG 14     ; Change memory location to 14
DB  30     ; Define a byte with the value 30 at memory location 14
DB  12     ; Define a byte with the value 12 at memory location 15
