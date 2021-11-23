            .text
            .global _start

_start:     ldw r4, TEST_NUM(r0)  
            mov r10, r0     #r10,r11,r12 0으로 초기화
            mov r11, r0
            mov r12, r0
            call ZEROS      #zeros 호출
            mov r10, r2     #return값을 r10에 저장
            call ONES       #ONES 호출
            mov r11, r2     #return값을 r10에 저장
            call ALTERNATE  #ALTERNATE 호출
            mov r12, r2     #return값을 r10에 저장
            br		END
            
ZEROS:      mov     r2,     r0          #r2 초기화
            nor     r4,     r4,     r0  #ONES와 같은 방식으로 구하기 위해 inverse를 해줌
            br		LOOP                #LOOP으로 branch
            
ONES:       mov     r2,     r0          #r2 초기화
            ldw     r4, TEST_NUM(r0)    #r4에 다시 TEST_NUM load
            br		LOOP                #LOOP으로 branch
            
LOOP:       beq		r4,		r0,		return  #ones(zeros)를 모두 구했다면 return
            srli    r13,    r4,     0x01    #r4 를 right shift
            and		r4,		r4,		r13     #shift한 값과 and
            addi    r2,     r2,     0x01    #return할 값에 +1
            br		LOOP                    #반복

ALTERNATE:  ldw     r4, TEST_NUM(r0)            #r4에 다시 TEST_NUM load            
            slli    r14,    r4,     31          #r4를 31byte left shift한다.
            bne     r4,     r0,     ALTERNATE1  #shift했을 때 값(원래 값의 마지막 byte자리)이 0이 아니라면 ALTERNATE1로 branch
            slli    r13,    r4,     0x01        #r4를 1byte만큼 left shift
            xor     r4,     r13,    r4          #shift된 값과 xor을 취한다
			movi     r2,     0x01               #시작지점부터 구해야하므로 1을 더해준다
            br      LOOP                        #xor취한 값의 ones를 구해야하므로 loop로 branch
ALTERNATE1: #slli하게되면 남는 자리는 0으로 채워지게 되므로 끝자리가 1이라면 alternate가 하나 더 생기는 것이므로 inverse해서 구해야한다.
            nor     r4,     r4,     r0      #r4를 inverse
            slli    r13,    r4,     0x01    #이하 ALTERNATE와 동일
            xor     r4,     r13,    r4
			movi    r2,     0x01
            br      LOOP

END:        br   END
return:     ret
TEST_NUM: .word 0x3FABEDEF /*an example*/
.end