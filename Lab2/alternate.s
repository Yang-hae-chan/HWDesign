            .text
            .global _start



_start:     ldw r4, TEST_NUM(r0)
            mov r12, r0
            call ALTERNATE
            mov r12, r2 /*r12 = count of the longest 10’s*/
            br		END
            
ONES:       mov     r2,     r0
            ldw     r4, TEST_NUM(r0)
OLOOP:      beq		r4,		r0,		return
            srli    r13,    r4,     0x01
            and		r4,		r4,		r13
            addi    r2,     r2,     0x01
            br		OLOOP
ALTERNATE:  ldw     r4, TEST_NUM(r0)
            slli    r13,    r4,     0x01
            xor     r4,     r13,    r4
			mov     r2,     0x01
            br      OLOOP


END:        br   END
return:     ret
TEST_NUM: .word 0x00F0FF00 /*an example*/
.end