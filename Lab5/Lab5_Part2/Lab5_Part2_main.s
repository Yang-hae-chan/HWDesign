    .include "./address_map_nios2.s"
    .text
    .global _start

_start: movia sp, SDRAM_END # stack pointer 주소를 설정
        movia r8,   LEDR_BASE # r8에 LEDR_BASE 주소를 저장
        call CONFIG_TIMER
        call CONFIG_KEYS
        /* enable Nios II processor interrupts */
        movi        r23,    3 	# ienable contrl register에 0b11 store
        wrctl	ienable,        r23
        movi        r23,    1
        wrctl	status,		r23
        /* initialize LEDR value */
        
         
LOOP:   ldw r9, COUNT(r0) /* global variable */
        stwio r9, (r8)     # r8에 COUNT store
        br LOOP

/* Configure the interval timer to create interrupts at 0.25 second intervals */
CONFIG_TIMER:
            movia   r22,    TIMER_BASE
            movui	r23,	0xE100     
            stwio   r23,    8(r22)      # 앞 16bit를 Timer의 Periodl에 store
            movui	r23,	0x05F5
            stwio   r23,    12(r22)     # 뒤 16bit를 Timer의 Periodh에 store
		    movi	r23,    0b0111 # start=1 cont=1 ito=1
            stwio   r23,    4(r22)
            ret
/* Configure the pushbutton KEYS to generate interrupts */
CONFIG_KEYS:
            movia   r22,    KEY_BASE    #KEY_BASE 주소를 저장
            movi	r23,	0b01111
            stwio   r23,    8(r22)
            ret
/* Global variables */
.global COUNT
COUNT: .word 0x0 # used by timer
.global RUN # used by pushbutton KEYs
RUN: .word 0x1 # initial value to increment COUNT
.end