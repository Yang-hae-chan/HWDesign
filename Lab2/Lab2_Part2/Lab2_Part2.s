            .text
            .global _start
_start:     
            ldw		r4,		TEST_NUM(r0)    #TEST_NUM 첫 번째 word load
            mov		r2,		    r0          #r2=0 초기화
            movia   r13,	TEST_NUM        #r13에 TEST_NUM 주소 복사

ONES:       
            beq     r4,     r0,      END    #TEST_NUM의 끝까지 오게되면 END
            mov		r10,	r0              #r10=0으로 초기화
            br      LOOP                    #LOOP으로 branch

ONES1:      addi    r13,    r13,    4       #다음 TEST_NUM주소로 이동
            ldw     r4,    (r13)            #r4에 다음 TEST_NUM load
            bgt     r2,     r10,    ONES    #앞서 LOOP에서 구한 연속된 1의 개수보다 이전 TEST_NUM에 구해서 저장했던 값이 더 크면 바로 ONES로 branch
            mov     r2,     r10             #아니라면 r2에 새로운 값을 저장
            br      ONES                    #ONES로 branch

LOOP:       beq		r4,		r0,		ONES1   #r4가 0이면 ONES1로 branch
            srli    r11,    r4,     0x01    #r11에 r4를 right shift한 값을 넣는다
            and		r4,		r4,		r11     #r4와 r4를 right shift한 값을 and
            addi    r10,    r10,    0x01    #r10=r10+1
            br		LOOP                    #LOOP으로 branch
END:        br		END
TEST_NUM:   .word		 0x000000FF, 0x00000001, 0x00000003,0x00000FAF,0x00000AAA,0x00101013,0x55555555,0xFFFFFFFF,0x00000005,0x000A0A01,0
            .end
            
            