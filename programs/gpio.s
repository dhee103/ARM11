;setup pin
ldr r2,=0x20200004  ;address for pin setup
mov r1,#1
lsl r1,#18
str r1,[r2]         ;set pin 16 as out

ldr r2,=0x20200028  ;clear address
ldr r3,=0x2020001C  ;set address

loop:
mov r1,#1
lsl r1,#16          ;r1 is used to write to set and clear addresses
str r1,[r2]         ;clear pin 16

mov r0,=0x1000
loop1:
sub r0,r0,#1
cmp r0,#0
bne loop1

str r1,[r3]

mov r0,=0x1000
loop2:
sub r0,r0,#1
cmp r0,#0
bne loop2

b loop
