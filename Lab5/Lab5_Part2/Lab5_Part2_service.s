.section .reset, "ax"
                        movia r2, _start
                        jmp r2
.section .exceptions, "ax" /*Allocatable and eXecutable.*/
.global ISR
                # sp 주소에 16을 감소시킴
ISR:            subi sp, sp, 16 /*et, ea, ra, r22 will be stored into memory*/
                stw et, 0(sp)   # 0(sp)에 exception temporary 저장
                rdctl et, ctl4 /*et is 0 only if no external (hardware) interrupts occurred; i.e. trap*/
                beq et, r0, SKIP_EA_DEC # ipending이 0이라면 ea-4 과정 skip
                subi ea, ea, 4 /*ea is decreased only for trap*/

SKIP_EA_DEC:    stw ea, 4(sp)   # ipending ctl register 데이터를 et에 저장
                stw ra, 8(sp)   # 8(sp)에 return address 저장
                stw r22, 12(sp) # 12(sp)에 r22 저장
                rdctl et, ctl4  # ipending ctl register 데이터를 et에 저장
                bne et, r0, ON_EXT_INT  # ipending 데이터가 0이면 END
ON_TRAP:        br END_ISR
ON_EXT_INT:     andi r22, et, 0b10 # et에 저장된 ipending data와 10을 and 시킴
                bne r22, r0, CALL_KEY   # and 값이 0이아니면 CALL_KEY로 br
                andi r22, et, 0b01 # et에 저장된 ipending data와 01을 and 시킴
                bne r22, r0, CALL_TIMER # and 값이 0이아니면 CALL_TIMER로 br
                br  END_ISR
CALL_TIMER:      call    ON_TIMER
                br  END_ISR
CALL_KEY:       call ON_KEY_PRESS
                br  END_ISR
END_ISR:        # 순서에 맞게 stack에 있는 데이터를 load 시킴
                ldw		et,		0(sp)/*et, ea, ra, r22 are restored from memory*/
                ldw		ea,     4(sp)
                ldw		ra,		8(sp)
                ldw		r22,	12(sp)
                addi		sp,		sp,		16
                eret
                .end
                