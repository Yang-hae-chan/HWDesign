            .include "address_map_nios2.s"
            .text
            .global _start

_start:     movia   r7, KEY_BASE        #r7에 KEY_BASE주소 복사
            movia   r8, HEX3_HEX0_BASE  #r8에 HEX3_HEX0_BASE주소 복사
            movia   sp,     0x20000     #sp에 주소값으로 0x20000복사
            movi    r9,		9
RST:        mov		r3,		r0          #r3(결과값) 초기화
                          
MAIN:               blt		r3,		r0,		RST
                    bgt     r3,     r9,     RST                 #output이 0~9범위 밖이면 리셋
KEY_PRESS_WAIT:     ldwio   r6,     0(r7)                       #r6에 key3~0상태 load
                    beq     r6,     r0,     KEY_PRESS_WAIT      #r6이 0이면 다시 KEY_PRESS_WAIT(아무것도 누르지 않았다면)
KEY_RELEASE_WAIT:   ldwio   r5,     0(r7)                       #r6에 key3~0상태 load
                    bne     r5,     r0,      KEY_RELEASE_WAIT   #r6이 0이 아니면 다시 KEY_PRESS_WAIT(누른채로 떼지 않았다면)

                    andi    r4,     r6,     0x1                 
                    bne     r4,     r0,     RESET_CALL          #key0이 눌렸다면 RESET_CALL
                    srli    r6,     r6,     0x1
                    andi    r4,     r6,     0x1
                    bne     r4,     r0,     INCREMENT_CALL      #key1이 눌렸다면 INCREMENT_CALL 
                    srli    r6,     r6,     0x1
                    andi    r4,     r6,     0x1
                    bne     r4,     r0,     DECREMENT_CALL      #key2이 눌렸다면 DECREMENT_CALL
                    srli    r6,     r6,     0x1
                    andi    r4,     r6,     0x1
                    bne     r4,     r0,     OFF_CALL            #key3이 눌렸다면 OFF_CALL
INCREMENT_CALL:     
                    call INCREMENT              #INCREMENT 호출
                    call    SEG7_CODE           #SEG7_CODE 호출
                    br MAIN                     #다시 MAIN으로
DECREMENT_CALL:     
                    call DECREMENT
                    call    SEG7_CODE 
                    br MAIN
RESET_CALL: 
                    call RESET
                    call    SEG7_CODE 
                    br MAIN
OFF_CALL: 
                    call OFF
                    br MAIN

INCREMENT:          addi    r3,     r3,     1       #결과값 1증가
                    bgt     r3,     r9,     RST     #10이상이면 다시 돌아감
					ret
DECREMENT:          subi    r3,     r3,     1       #결과값 1감소
                    blt		r3,		r0,		RST     #0미만이면 다시 돌아감
					ret
RESET:              mov		r3,		r0              #결과값 초기화
                    ret
OFF:                subi    sp,     sp,     4       #call된 서브루틴 내에서 다시call하므로 return할 주소를 저장해야한다
                    stw		ra,		(sp)
                    call RESET
                    stwio   r0,     (r8)            #HEX OFF  
                    ldw		ra,		(sp)            #저장해놓았던 return할 주소 다시 load
                    addi    sp,     sp,     4       #sp 원상복구
                    ret

BIT_CODES:	        .byte	0b00111111,0b00000110,0b01011011,0b01001111,0b01100110
		            .byte	0b01101101,0b01111101,0b00000111,0b01111111,0b01100111
		            .skip	2   #숫자가 총 10개이므로 10byte이다. word단위로 만들어주기 위해 2byte skip
SEG7_CODE:	        movia	r15,BIT_CODES   #BIT_CODES 주소 복사
		            add	    r15,r15,r3          #r3의 숫자에 맞게 주소값에 더해서 해당 bitcode 주소를 가질 수 있게한다.
		            ldb	    r2,(r15)            #r2에 load
                    stwio   r2,     (r8)        #r8에 store
                    ret                         #return
