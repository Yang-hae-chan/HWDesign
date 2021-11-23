            .text
            .global _start

_start:
            movia   r8, RESULT      #r8 -> result 주소
            ldw     r4, 4(r8)       #r4에  r8+4에 있는(N)불러옴
            addi	r5,	r8, 8       #r5 -> NUMBERS 주소
            call		LARGE       #LARGE 호출
            stw     r2, (r8)        #r2값을 r8에 저장
            ldw     r7, (r8)

STOP:       br      STOP

LARGE:      ldw     r2,     (r5)            #r2에 r5값을 load
LOOP:       subi	r4,		r4,		1       #for와 같은역할
            beq		r4,		r0,		DONE    
            addi    r5,     r5,     4       #다음숫자로 이동
            ldw     r6,     (r5)            #r6에 r5의 값을 load
            bge		r2,		r6,		LOOP    #r2값이 r6보다 크면 LOOP로 이동
            mov		r2,		r6              #r2=r6
            br      LOOP
DONE:       ret                             #return address로 돌아감

RESULT:     .skip   4                       #저장할 공간
N:          .word   7                       #숫자의 개수
NUMBERS:    .word	4, 5, 3, 6              #숫자 4개 단위로 저장
            .word   1, 8, 2

            .end    

        