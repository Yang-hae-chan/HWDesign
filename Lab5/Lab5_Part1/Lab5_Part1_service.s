.section    .reset, "ax"
                        movia r2, _start
                        jmp r2
.section .exceptions, "ax" /*Allocatable and eXecutable.*/
.global ISR

ISR:    subi    sp,     sp,     16      # 스택에 et를 저장
        stw	et,	0(sp)
        rdctl	et,	ctl4            # HW interrupt인지 확인
        beq	et,	r0,	SKIP_EA_DEC     # SW interrupt
        subi    ea,     ea,     4               # HW interrupt의 경우 실행하지 못했던 interrupt 이전 명령어를 실행

SKIP_EA_DEC:    stw     ea,     4(sp)   # exception handling에 사용되는 레지스터 백업
                stw     ra,     8(sp)   
                stw     r22,    12(sp)
                rdctl   et,     ctl4    #ipending read
                bne	et,	r0,	ON_EXT_INT      
                #SW interrup->on_trap HW->on_ext_int
ON_TRAP:        br      END_ISR

ON_EXT_INT:     andi	r22,	et,	0b10    #key에 의한것인지?
                beq     r22,    r0,     END_ISR #아니라면 end
                call	ON_KEY_PRESS            #맞다면 call ON_KEY_PRESS

END_ISR:        ldw	et,	0(sp)           # 데이터 복구
                ldw	ea,     4(sp)
                ldw	ra,	8(sp)
                ldw	r22,	12(sp)
                addi	sp,	sp,	16
                eret
                .end
                
                
                
                

           
        