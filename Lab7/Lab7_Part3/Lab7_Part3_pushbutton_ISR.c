#include "address_map_nios2.h"
extern int RUN;                 //main code에서 사용한 전역변수 RUN
extern volatile int *KEY_ptr;   //main code에서 사용한 전역변수
extern volatile int *TIMER_ptr; //main code에서 사용한 전역변수
extern int timevalue;           //main code에서 사용한 전역변수
void pushbutton_ISR(void){
    int KEY_edge=*(KEY_ptr+3);  //KEY edgecapture
    *(KEY_ptr+3)=KEY_edge;      //KEY edgecapture 초기화
    if(KEY_edge==0b1000){       //KEY 3번이 눌리면
        RUN=~RUN;               //RUN 토글
    }
    else if(KEY_edge==0b0100){  //KEY 2번이 눌리면
        timevalue=timevalue*2;  //timevalue두배
        *(TIMER_ptr+2)=timevalue;       //변경된 timevalue에 맞게 timer재설정
        *(TIMER_ptr+3)=timevalue>>16;
        *(TIMER_ptr+1)=0b0111;
    }
    else if(KEY_edge==0b0010){ //KEY 1번이 눌리면
        timevalue=timevalue/2;  //timevalue0.5배
        *(TIMER_ptr+2)=timevalue;   //변경된 timevalue에 맞게 timer재설정
        *(TIMER_ptr+3)=timevalue>>16;
        *(TIMER_ptr+1)=0b0111; 
    }  
    return;
}