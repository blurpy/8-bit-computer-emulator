OUT        ; Output the value of the A-register
ADD 15     ; Put the value from memory location 15 in the B-register, and store A+B in the A-register
JC   4     ; Jump to instruction 4 if the A-register is past 255
JMP  0     ; Jump to instruction 0
SUB 15     ; Put the value from memory location 15 in the B-register, and store A-B in the A-register
OUT        ; Output the value of the A-register
JZ   0     ; Jump to instruction 0 if the A-register is 0
JMP  4     ; Jump to instruction 4
ORG 15     ; Change memory location to 15
DB   1     ; Define a byte with the value 1 at memory location 15
