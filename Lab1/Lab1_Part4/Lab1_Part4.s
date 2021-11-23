            .text
            .global _start

_start:     
            movia   r4,     N               #r4에 N 주소값
            addi    r8,     r4,     4       #r8에 digits 주소값 저장
            ldw		r4,		(r4)            #r4에 N load
            mov     r9,     r8              #r9에 r8의 주소값을 복사해놓는다
LOOP:
            call		DIVIDE              #DIVIDE호출
            stb     r2,     (r8)            #반환된 r2값 r8에 store
            mov     r4,     r3              #다음 차례에 r2에 넣을 값을 r4에 저장해놓는다.
            addi    r8,     r8,     1       #다음 자리를 저장하기 위해 주소값 +1
            bge		r4,		r5,		LOOP    #r4가 10보다 크다면 반복
            stb     r3,     (r8)            #r3를 r8에 store
            ldw     r7,     (r9)            #결과값을 확인하기 위해 넣음

END:    br		END

DIVIDE:
            mov		r2,		r4  #r2=나머지
            movi    r5,     10  #r5=divisor
            movi    r3,     0   #r3=몫
CONT:      
            blt		r2,		r5,		DIV_END #r2<r5면 종료(나눌 숫자가 10미만이면 종료)
            sub		r2,		r2,		r5      #r2=r2-10
            addi    r3,     r3,     1       #몫 계산
            br      CONT
DIV_END:    ret

N:          .word   1234
Digits:     .space  4

            .end