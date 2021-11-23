            .text
            .global _start

_start:     mov     r2,     r0              #r2(sum저장할 레지스터)초기화
            movia   r13,	N               #r13에 N주소값 저장
            ldw		r4,		0(r13)          #r4에 N에 저장된 숫자 load
            call		FINDSUM             #FINDSUM 호출

END:        br		END 

FINDSUM:    bne		r4,		r0,		LOOP    #r4가 0이 아니라면 LOOP으로 br
            ret                             #call FINDSUM 뒤로 return
LOOP:       add		r2,		r2,		r4      #r2=r2+r4(sum(N)=sum(N-1)+N)
            subi	r4,		r4,		1       #N=N-1
            br      FINDSUM                 #FINDSUM 반복

N:          .word   9
            .end