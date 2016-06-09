;setup
ldr r0,=0x20200000
mov r1,#1
mov r3,#0 ;serves no purpose but to identify counter register
;22,27,17 outs
lsl r1,#21
str r1,[r0,#4]
add r2,r1
lsr r1,#15
add r2,r1
str r2,[r0,#8]

;23,24 ins

loop:
;read input and branch to up or down
ldr r1,[r0, #0x34]
mov r2,#1
lsl r2,#23
tst r1,r2
be up
lsl r2,#1
tst r1,r2
be down
b loop


up:
mov r1,#1		;just testing first read
lsl r1,#22         ;r1 is used to write to set and clear addresses
str r1,[r0,#0x1c]         ;clear pin 16

add r3,#1

b update

down:
sub r3,#1
b update

update:
//adjust leds
b loop
