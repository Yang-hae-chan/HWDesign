            .text
            .global _start
_start:
            ldw		r4,		TEST_NUM(r0)

            mov		r10,		r0
ONES:
            beq		r4,		r0,		END
            srli    r11,    r4,     0x01
            and		r4,		r4,		r11
            addi    r10,    r10,    0x01
            br		ONES
END:
            br		END
TEST_NUM:
            .word		0x3fabedefa
            .end
            
            