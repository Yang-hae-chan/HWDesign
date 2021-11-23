            .text
            .global _start

_start:     movia   r13,	N                   #r13에 N주소값 복사
            movia   sp,     0x20000             #sp에 주소값으로 0x20000복사
            ldw		r4,		0(r13)              #r4에 N에 저장된 숫자 load
            call		FINDSUM                 #FINDSUM호출

END:        br		END

FINDSUM:    bne		r4,		r0,		RECURSE     #r4(N)이 0이아니면 RECURSE로 br
            mov     r2,     r0                  #r4(N)이 0이면 r2에 0복사
            ret                                 #RECURSE내 FINDSUM호출 뒤로 return

RECURSE:    subi	sp,		sp,		8           #sp에 r4와 ra를 저장하기위해 8byte를 뺀다
            stw		r4,		0(sp)               #sp에 r4 store
            stw		ra,		4(sp)               #sp에 ra store
            subi    r4,     r4,     1           #N=N-1
            call    FINDSUM                     #FINDSUM 호출
            ldw     r4,     0(sp)               #sp에 저장했던 r4값 load
            ldw     ra,     4(sp)               #sp에 저장했던 ra값 load
            addi	sp,		sp,		8           #저장했던 값을 다시 load했으므로 뺀 공간만큼 더해서 없애줌
            add     r2,     r4,     r2          #SUM(N)=N+SUM(N-1)
            ret                                 #맨처음 FINDSUM 호출한 뒤로 return
            

N:          .word   4
            .end