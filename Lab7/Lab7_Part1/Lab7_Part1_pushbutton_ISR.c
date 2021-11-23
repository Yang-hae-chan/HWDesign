#include "address_map_nios2.h"
extern volatile int *KEY_ptr; // main.c 에서 선언했던 KEY_BASE 에 대한 포인터 변수
extern int HEX_Output;		  // main.c 에서 선언했던 HEX3_HEX0 에 저장될 변수
extern unsigned int HEX3,HEX2,HEX1,HEX0;
unsigned int seg_code[] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111,
                            0b01100110, 0b01101101, 0b01111101, 0b00000111,
                            0b01111111, 0b01100111 };//HEX출력을 위한 segcode
void pushbutton_ISR(void){
    int KEY_edge=*(KEY_ptr+3);//KEY edgecapture
    *(KEY_ptr+3)=KEY_edge;//edgecapture초기화
    if(KEY_edge==0b1000){//key3번이 눌리면
        if(HEX3==0)//이전에 OFF된 상태라면
        HEX3=seg_code[3];//hex3에 3 on
        else
        HEX3=0;         //아니라면 off
    }
    else if(KEY_edge==0b0100){//key2번이 눌리면
        if(HEX2==0)//이전에 OFF된 상태라면
        HEX2=seg_code[2];//hex2에 2 on
        else
        HEX2=0;      //아니라면 off   
    }
    else if(KEY_edge==0b0010){//key1번이 눌리면
        if(HEX1==0)//이전에 OFF된 상태라면
        HEX1=seg_code[1];//hex1에 1 on
        else
        HEX1=0;         //아니라면 off
    }
    else if(KEY_edge==0b0001){//key0번이 눌리면
        if(HEX0==0)//이전에 OFF된 상태라면
        HEX0=seg_code[0];//hex0에 0 on
        else
        HEX0=0;   //아니라면 off     
    }
    HEX_Output=(HEX3<<24)|(HEX2<<16)|(HEX1<<8)|(HEX0); 
    //HEX_Output에 각각의 HEX출력값들을 합쳐서 저장 
    return;
}