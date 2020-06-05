;File with errors test
.data -21

FREAK: stop #12,*r7,bsr

				
MAIN: add r3,LIST
LOOP: prn #48
lea STR,r6
inc r6,#42
mov *r6,*K
sub r1
cmp r3,#-6
bnedfg END
dec K
jmp 
LOOP bne jsr ffffff
END: stop
STR: .string "abcd"
LIST: .data 6,-9
.data -100
An intentional mistake
K: .data 31
