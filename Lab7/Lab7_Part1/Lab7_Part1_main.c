#include <stdio.h>
#include "nios2_ctrl_reg_macros.h"
#include "address_map_nios2.h"

volatile int *KEY_ptr = (int*)KEY_BASE;
volatile int *HEX3_HEX0_ptr = (int*)HEX3_HEX0_BASE;//각각의 BASSE에 맞는 포인터
unsigned int HEX_Output=0;//HEX 초기화, 각 HEX출력값 저장하기 위한 전역변수
unsigned int HEX3=0,HEX2=0,HEX1=0,HEX0=0;//각 HEX출력값 초기화
void enable_nios2_interrupts(void); //interrupt를 enable하게 할 함수 선언
void config_KEYs(void);// KEY초기 설정 함수
void main(void){
    config_KEYs(); // configure pushbutton KEYs to generate interrupts
    enable_nios2_interrupts(); // enable interrupts in the Nios II processor
    *HEX3_HEX0_ptr=0;           //HEX OFF
    while (1){
        *HEX3_HEX0_ptr=HEX_Output;
    } // wait for an interrupt
}
/* Set up the pushbutton KEYs port in the FPGA */
void config_KEYs(void){
    *(KEY_ptr + 2)=0b01111;     //KEY0-3사용
}
/* Enable interrupts in the Nios II processor */
void enable_nios2_interrupts(void){
    NIOS2_WRITE_IENABLE(0x02);   //IRQ pushbutton
    NIOS2_WRITE_STATUS(0b01);      //interrupt.PIE=1
}