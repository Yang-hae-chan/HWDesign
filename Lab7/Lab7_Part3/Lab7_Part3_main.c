#include <stdio.h>
#include "nios2_ctrl_reg_macros.h"
#include "address_map_nios2.h"

int COUNT = 0; // global counter for red lights
int RUN = 1; // global, used to increment/not the count variable
int timevalue=25000000;//초기값 0.25
volatile int *KEY_ptr = (int*)KEY_BASE;
volatile int *LEDR_ptr = (int*)LEDR_BASE;
volatile int *TIMER_ptr = (int*)TIMER_BASE;//각각의 BASE에 맞는 포인터
void enable_nios2_interrupts(void);
void config_KEYs(void);
void config_timer(void);
void main(void){
    config_KEYs(); // configure pushbutton KEYs to generate interrupts
    config_timer();
    enable_nios2_interrupts(); // enable interrupts in the Nios II processor
    *LEDR_ptr=0; 
    while (1){
        *LEDR_ptr=COUNT;
    } // wait for an interrupt
}
/* Set up the pushbutton KEYs port in the FPGA */   
void config_KEYs(void){
    *(KEY_ptr + 2)=0b01110;     //KEY3-1사용
}
/* Enable interrupts in the Nios II processor */
void enable_nios2_interrupts(void){
    NIOS2_WRITE_IENABLE(0b011);   //IRQ pushbutton
    NIOS2_WRITE_STATUS(0b01);      //interrupt.PIE=1
}
void config_timer(void){
    int pl=timevalue;//timevalue에 맞게 설정
    int ph=pl>>16;
    *(TIMER_ptr+2)=pl;
    *(TIMER_ptr+3)=ph;
    *(TIMER_ptr+1)=0b0111;//stop,start,cont,ito=0111
}