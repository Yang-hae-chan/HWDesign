    .include "./address_map_nios2.s"
    .text
    .global _start
_start:
        movia sp, SDRAM_END             # SP에 SDRAM_END 주소 복사
        movia r22, HEX3_HEX0_BASE
        stwio r0, 0(r22)
        
        movia   r22,  KEY_BASE          # r22에 KEY_BASE 주소 복사
        movi	r23,	0b01111         # mask 0b01111(key0~3까지 사용)
        stwio   r23,    8(r22)          # interrupt mask 레지스터를 0b1111 설정

        movi    r23,    2               # ienable을 0b10 설정    
        wrctl	ienable,r23             # ienable을 0b10 설정
        movi    r23,    1
        wrctl	status,	r23             # status를  0b1 설정

IDLE:
        br IDLE
        .end
