            .text
            .global _start

_start:     movi    r5,     1           #FINDFACT에서 조건문에서 사용할 1
            movia   r13,	N           #r13에 N주소값 복사
            movia   sp,     0x20000     #sp에 주소값으로 0x20000복사
            ldw		r4,		0(r13)      #r4에 N에 저장된 숫자 load
            call		FINDFACT        #FINDFACT호출

END:        br		END

FINDFACT:   bne		r4,		r5,		RECURSE     #r4(N)가 1이 아니면 RECURSE호출
            movi    r2,     1                   #r2=1
            ret                                 #recurse 내에서 FINDFACT호출한 뒤로 return

RECURSE:    subi	sp,		sp,		8           #sp에 r4와 ra를 저장하기위해 8byte를 뺀다
            stw		r4,		0(sp)               #sp에 r4 store
            stw		ra,		4(sp)               #sp에 ra store
            subi    r4,     r4,     1           #N=N-1
            call    FINDFACT                    #FINDFACT 호출
            ldw     r4,     0(sp)               #sp에 저장했던 r4값 load
            ldw     ra,     4(sp)               #sp에 저장했던 ra값 load
            addi	sp,		sp,		8           #저장했던 값을 다시 load했으므로 뺀 공간만큼 더해서 없애줌
            mul		r2,		r4,		r2          #FACT(N)=N*FACT(N-1)
            ret                                 #맨처음 FINDFACT 호출한 뒤로 return
            

N:          .word   4
            .end