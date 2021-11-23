    .include "./address_map_nios2.s"
    .global ON_TIMER


ON_TIMER:       subi sp, sp, 8
                stw r22, 0(sp)
                stw r23, 4(sp)
                movia r22, TIMER_BASE   # r22에 TIMER_BASE 주소 할당
                stwio	r0,     (r22)         #TO를 다시 0으로
                movia   r22,    RUN     # global. RUN에 저장되어 있는 수 확인
                ldw     r23,    (r22)   #global. RUN에 저장되어 있는 수 load
                beq		r23,	r0,		END_ON_TIMER    #RUN이 0이면 end
                movia   r22,    COUNT       # RUN이 1이면 COUNT
                ldw     r23,    (r22)       # RUN이 1이면 COUNT
                addi    r23,     r23,    1 /*Increment*/
                stw     r23,    (r22) /*Turn off IRQ by writing a value to the edgecapture register*/
                
END_ON_TIMER:   ldw r22, 0(sp)
                ldw r23, 4(sp)
                addi sp, sp, 8
                ret
                .end