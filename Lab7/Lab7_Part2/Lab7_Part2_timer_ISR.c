#include "address_map_nios2.h"
extern volatile int *TIMER_ptr; // main.c 에서 선언했던 TIMER_BASE 에 대한 포인터 변수
extern int COUNT; // main.c 에서 선언했던 변수
extern int RUN;     // main.c 에서 선언했던 변수
void timer_ISR(void){
    if(RUN&*TIMER_ptr&0b1){//RUN이1이고 TO가 1일때
        *TIMER_ptr=0;//TO 초기화
        COUNT++;//COUNT함
    }  
    return;
}