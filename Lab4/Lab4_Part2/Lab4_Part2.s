            .include "address_map_nios2.s"
            .text
            .global _start

_start:     
            movia   r8, HEX3_HEX0_BASE  #r8에 HEX3_HEX0_BASE주소 복사
            movia   r9, TIMER_BASE      #r9에 TIMER_BASE주소 복사
            movia   sp,     0x20000     #sp에 주소값으로 0x20000복사
            mov		r7,		r0          #결과값 초기화
            movui	r10,	0xE100     
            stwio   r10,    8(r9)
            movui	r10,	0x05F5
            stwio   r10,    12(r9)      #r9 {periodh, periodl}에 100,000,000 저장
            movui	r10,	0x06
            stwio   r10,    4(r9)       #start=1 cont=1
			movi	r11,	0x063       #99
                                      
MAIN:
TIMER_TO_WAIT:      ldwio   r6,    (r9)
					andi	r6,		r6,		0x01
                    beq     r6,     r0,     TIMER_TO_WAIT       #TO가 1이 아니면 다시 TIMER_TO_WAIT                            
                    stbio	r0,    (r9)                         #TO를 다시 0으로
                    br      INCREMENT_CALL                      #TO가 1이 될때마다 INCREMENT_CALL분기
INCREMENT_CALL:     
                    call INCREMENT
                    call    DISPLAY 
                    br MAIN

INCREMENT:          addi    r7,     r7,     1               #output+1
					bgt		r7,		r11,	INCREMENT1      #output이 99보다 크면 INCREMENT1
                    ret
INCREMENT1:			mov		r7,		r0                      #output 초기화
					ret

DIVIDE:	
		movi	r5,10       
		movi	r3,0       #십의 자리수

CONT:	blt	    r2,r5,return   #r2가 10보다 작으면 return, r2는 1의 자리 수
		sub	    r2,r2,r5       #r2에 10을 뺄 때마다 r3에 더함으로써 r3이 10의 자리수를 가질 수 있다.
		addi    r3,r3,1
		br	    CONT    
return: ret
BIT_CODES:	        .byte	0b00111111,0b00000110,0b01011011,0b01001111,0b01100110
		            .byte	0b01101101,0b01111101,0b00000111,0b01111111,0b01100111
		            .skip	2   #숫자가 총 10개이므로 10byte이다. word단위로 만들어주기 위해 2byte skip
SEG7_CODE:	        movia	r15,BIT_CODES   #BIT_CODES 주소 복사
		            add	    r15,r15,r4          #r4의 숫자에 맞게 주소값에 더해서 해당 bitcode 주소를 가질 수 있게한다.
		            ldb	    r2,(r15)            #r2에 load
					ret
DISPLAY:    subi    sp,     sp,     4   #call된 서브루틴 내부에서 다시 call하므로 return할 주소 저장
            stw		ra,		(sp)
            mov		r2,		r7          #output r2로 복사
            call     DIVIDE             #DIVIDE 호출
            mov     r4,     r2          #r4에 r2 복사(1의 자리 수)
            call    SEG7_CODE           #1의 자리 수에 맞게 mapping
            mov     r14,    r2          #return받은 값을 r14에 복사
            mov     r4,     r3          #r4에 r3 복사(10의 자리 수)
            call    SEG7_CODE           #10의 자리 수에 맞게 mapping
            slli	r2,		r2,		8   
            or      r14,    r14,    r2  #r2와 r3에 저장된 값을 r14에 하나로 이어서 저장
            stwio   r14,    (r8)        #r8에 store(HEX에 출력)
            ldw		ra,		(sp)        #return하기위해 저장했던 주소 load
            addi    sp,     sp,     4   #sp 원상복구
            ret