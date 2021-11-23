            .text
            .global _start
_start:     ldw		r4,		TEST_NUM(r0)    #testnum load
            mov		r2,		    r0          #r2 초기화
            movia   r13,	TEST_NUM        #r13에 TEST_NUM주소 복사
            call    ONES                    #ONES 호출
            call    DISPLAY                 #DISPLAY 호출

ONES:       beq     r4,     r0,    return   #TEST_NUM의 끝까지 오게되면 END
            mov		r10,	r0              #r10=0으로 초기화
            br      LOOP                    #LOOP으로 branch

ONES1:      addi    r13,    r13,    4       #다음 TEST_NUM주소로 이동
            ldw     r4,    (r13)            #r4에 다음 TEST_NUM load
            bgt     r2,     r10,    ONES    /*앞서 LOOP에서 구한 연속된 1의 개수보다
            이전 TEST_NUM에 구해서 저장했던 값이 더 크면 바로 ONES로 branch*/
            mov     r2,     r10             #아니라면 r2에 새로운 값을 저장
            br      ONES                    #ONES로 branch

LOOP:       beq		r4,		r0,		ONES1   #r4가 0이면 ONES1로 branch
            srli    r11,    r4,     0x01    #r11에 r4를 right shift한 값을 넣는다
            and		r4,		r4,		r11     #r4와 r4를 right shift한 값을 and
            addi    r10,    r10,    0x01    #r10=r10+1
            br		LOOP                    #LOOP으로 branch

# 숫자별로 HEX LED에 불이 ON인지 OFF인지를 binary 로 나타낸 것
BIT_CODES:	.byte	0b00111111,0b00000110,0b01011011,0b01001111,0b01100110
		    .byte	0b01101101,0b01111101,0b00000111,0b01111111,0b01100111
		    .skip	2   #숫자가 총 10개이므로 10byte이다. word단위로 만들어주기 위해 2byte skip
SEG7_CODE:	movia	r15,BIT_CODES   #BIT_CODES 주소 복사
		    add	r15,r15,r4          #r4의 숫자에 맞게 주소값에 더해서 해당 bitcode 주소를 가질 수 있게한다.
		    ldb	r2,(r15)            #r2에 load
		    br  return              #return
DIVIDE:	
		movi	r5,10       
		movi	r3,0       #십의 자리수

CONT:	blt	r2,r5,return   #r2가 10보다 작으면 return, r2는 1의 자리 수
		sub	r2,r2,r5       #r2에 10을 뺄 때마다 r3에 더함으로써 r3이 10의 자리수를 가질 수 있다.
		addi	r3,r3,1
		br	CONT

return:    ret  
DISPLAY:    movia	r8,0xFF200020      #r8에 HEX1-0 주소값을 복사
            call     DIVIDE             #DIVIDE 호출
            mov     r4,     r2          #r4에 r2 복사(1의 자리 수)
            call    SEG7_CODE           #1의 자리 수에 맞게 mapping
            mov     r14,    r2          #return받은 값을 r14에 복사
            mov     r4,     r3          #r4에 r3 복사(1-의 자리 수)
            call    SEG7_CODE           #10의 자리 수에 맞게 mapping
            slli	r2,		r2,		8   
            or      r14,    r14,    r2  #r2와 r3에 저장된 값을 r14에 하나로 이어서 저장
            stw     r14,    (r8)        #r8에 store
END:        br		END
TEST_NUM:   .word		0x000000FF, 0x00000001, 0x00000003,0x00000FAF,0x00000AAA,0x00101013,0x55555555,0xFFFFFFFF,0x00000005,0x000A0A01,0
            .end
            
            