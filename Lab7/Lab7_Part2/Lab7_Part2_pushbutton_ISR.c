#include "address_map_nios2.h"
extern int RUN; //main code에서 사용한 전역변수 RUN
extern volatile int *KEY_ptr; //main code에서 사용한 전역변수
void pushbutton_ISR(void){
    int KEY_edge=*(KEY_ptr+3);//KEY edgecapture
    *(KEY_ptr+3)=KEY_edge;//edgecapture초기화
    if(KEY_edge==0b1000){//key3번이 눌리면
        RUN=~RUN;       //RUN 토글
    }  
    return;
}