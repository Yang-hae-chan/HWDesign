    .include "./address_map_nios2.s"
    .global ON_KEY_PRESS


ON_KEY_PRESS:   subi sp, sp, 8  # sp 주소를 감소시킨후 사용할 register를 stack에 저장
                stw r22, 0(sp)
                stw r23, 4(sp)
                movia r22, KEY_BASE
                ldwio r23, 0xC(r22) # KEY_BASE의 edgecapture load
                stwio r23, 0xC(r22) /*Turn off IRQ by writing a value to the edgecapture register*/
                movia r22, RUN      # global. RUN에 저장되어 있는 수 확인 
                ldw		r23,    (r22)    # global. RUN에 저장되어 있는 수 load
                xori    r23,    r23,    0b01    # 0b01과 r22를 xor 연산하여 데이터를 toggle
                stw     r23,    (r22)   #RUN 업데이트
                ldw r22, 0(sp)  #백업데이터 복구
                ldw r23, 4(sp)
                addi sp, sp, 8
                ret
                .end
                