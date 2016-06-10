;setup base address
ldr r0,=0x20200000

;set GPIO 16 as output pin
mov r1,#1
lsl r1,#18
str r1,[r0,#4]

;set bit 16
mov r1,r1,lsr #2

; main loop
loop:

;clear GPIO 16
str r1,[r0,#40]

mov r2,#0xF0000
delay:
sub r2,r2,#1
cmp r2,#0
bne delay

;set GPIO 16
str r1,[r0,#28]

mov r2,#0xF0000
delay2:
sub r2,r2,#1
cmp r2,#0
bne delay2

b loop
