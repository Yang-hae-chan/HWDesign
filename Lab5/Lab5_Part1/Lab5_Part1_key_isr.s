.include "./address_map_nios2.s"
.global ON_KEY_PRESS

ON_KEY_PRESS:   subi    sp,     sp, 8           # 데이터 백업
                stw     r22,    0(sp)
                stw     r23,    4(sp)
                movia   r22,    KEY_BASE
                ldwio   r23,    0xC(r22) 
                stwio   r23,    0xC(r22) /*Turn off IRQ by writing a value to the edgecapture register*/
                movia   r22,    HEX3_HEX0_BASE
                movi	r15,	0b00111111      #0 SEGCODE
                movi	r16,	0b00000110      #1 SEGCODE
                movi	r17,	0b01011011      #2 SEGCODE
                movi	r18,	0b01001111      #3 SEGCODE
                slli	r16,	r16,	8       #HEX1로 출력하기위해 8BIT SHIFT
                slli	r17,	r17,	16      #HEX2로 출력하기위해16BIT SHIFT
                slli	r18,	r18,	24      #HEX3로 출력하기위해24BIT SHIFT
                #Edge capture와 비교해 몇번째 key인지 확인
                andi	r19,	r23,	0b1000
                bne		r19,	r0,		KEY3
                andi	r19,	r23,	0b0001
                bne		r19,	r0,		KEY0
                andi	r19,	r23,	0b0010
                bne		r19,	r0,		KEY1
                andi	r19,	r23,	0b0100
                bne		r19,	r0,		KEY2
                
                # 데이터 복구
ON_KEY_PRESS1:  ldw     r22,    0(sp)
                ldw     r23,    4(sp)
                addi    sp,     sp,     8
                ret
                # KEY0일때
KEY0:           ldwio   r23,    0(r22) /*Read the previous value*/
                and		r23,	r23,	r15     #이전에 0이 켜진상태이면
                beq		r23,	r15,	OFF0    #켜져있었다면 0을 끈다
                ldwio   r23,    0(r22)          #꺼져있었다면 다시 load
                add		r23,	r23,    r15     #0 ON
                stwio   r23,    0(r22) /*Write the new value*/
                br  ON_KEY_PRESS1
                # KEY1일때
KEY1:           ldwio   r23,    0(r22) /*Read the previous value*/
                and		r23,	r23,	r16     #이전에 1이 켜진상태이면
                beq		r23,	r16,	OFF1    #켜져있었다면 1을 끈다
                ldwio   r23,    0(r22)          #꺼져있었다면 다시 load
                add     r23,    r23,    r16     #1 ON
                stwio   r23,    0(r22) /*Write the new value*/
                br  ON_KEY_PRESS1
                # KEY2일때
KEY2:           ldwio   r23,    0(r22) /*Read the previous value*/
                and		r23,	r23,	r17     #이전에 2이 켜진상태이면
                beq		r23,	r17,	OFF2    #켜져있었다면 2을 끈다
                ldwio   r23,    0(r22)          #꺼져있었다면 다시 load
                add     r23,    r23,    r17     #2 ON
                stwio   r23,    0(r22) /*Write the new value*/
                br  ON_KEY_PRESS1
                # KEY3일때
KEY3:           ldwio   r23,    0(r22) /*Read the previous value*/
                and		r23,	r23,	r18     #이전에 3이 켜진상태이면
                beq		r23,	r18,	OFF3    #켜져있었다면 3을 끈다
                ldwio   r23,    0(r22)          #꺼져있었다면 다시 load
                add     r23,    r23,    r18     #3 ON
                stwio   r23,    0(r22) /*Write the new value*/
                br  ON_KEY_PRESS1
OFF0:           ldwio   r23,    0(r22)          #0 OFF
                sub     r23,    r23,     r15
                stwio   r23,    0(r22)
                br  ON_KEY_PRESS1
OFF1:           ldwio   r23,    0(r22)          #1 OFF
                sub     r23,    r23,     r16
                stwio   r23,    0(r22)
                br  ON_KEY_PRESS1
OFF2:           ldwio   r23,    0(r22)          #2 OFF
                sub     r23,    r23,     r17
                stwio   r23,    0(r22)
                br  ON_KEY_PRESS1
OFF3:           ldwio   r23,    0(r22)          #3 OFF
                sub     r23,    r23,     r18
                stwio   r23,    0(r22)
                br  ON_KEY_PRESS1
                .end
