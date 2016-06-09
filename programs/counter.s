setup:

ldr r0,=0x20200000
mov r1,#1

;22,27,17 outs
lsl r1,#21
str r1,[r0,#4]
add r2,r1
lsr r1,#15
add r2,r1
str r2,[r0,#8]

;23,24 ins

loop:
;read input(change), update counter, display count, wait, loop
ldr r1,[r0, #0x34]
mov r2,#1
lsl r2,#23
tst r1,r2
be on
b loop

on:
mov r1,#1
lsl r1,#22         ;r1 is used to write to set and clear addresses
str r1,[r0,#0x1c]         ;clear pin 16

mov r0,=0x1000
loop1:
sub r0,r0,#1
cmp r0,#0
bne loop1

str r1,[r0,#x28]
b loop
