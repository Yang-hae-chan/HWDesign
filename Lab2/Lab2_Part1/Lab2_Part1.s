.text 
.global _start

_start:     ldw	r9,TEST_NUM(r0)      /*임의의 값을 r9에 저장*/
            mov    r10,r0                /* r10 0으로 초기화*/

ONES:		beq	r9,r0,END                /* r9=0이면, 루프 종료*/
		    srli	r11,r9,1                /* r9를 오른쪽으로 SHIFT*/
		    and	r9,r9,r11                /* SHIFT 한 값과 원래 값 AND*/
		    addi	r10,r10,1                /* 카운터인, r10값 증가*/
		    br 	ONES

END:		br	END
TEST_NUM:	.word	0xFFFFFFFF             
		    .end
