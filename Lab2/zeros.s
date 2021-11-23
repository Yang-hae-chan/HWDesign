            .text
            .global _start
_start:
            ldw		r4,		TEST_NUM(r0)
            mov		r11,		r0
           
ZEROS:      
            nor     r4,     r4,     r0
LOOP:       beq		r4,		r0,		END
            srli    r13,    r4,     0x01
            and		r4,		r4,		r13
            addi    r11,    r11,    0x01
            br		LOOP
END:
            br		END
TEST_NUM:
            .word		0xFFFFFFF0
            .end
            
            
            