#include <stdio.h>
#include "nios2_ctrl_reg_macros.h"
#include "address_map_nios2.h"

int COUNT = 0; // global counter for red lights
int RUN = 1; // global, used to increment/not the count variable
volatile int *KEY_ptr = (int*)KEY_BASE;
volatile int *LEDR_ptr = (int*)LEDR_BASE;
volatile int *TIMER_ptr = (int*)TIMER_BASE;//각각의 BASSE에 맞는 포인터
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
    *(KEY_ptr + 2)=0b01000;     //KEY3사용
}
/* Enable interrupts in the Nios II processor */
void enable_nios2_interrupts(void){
    NIOS2_WRITE_IENABLE(0b011);   //IRQ pushbutton,TIMER
    NIOS2_WRITE_STATUS(0b01);      //interrupt.PIE=1
}
void config_timer(void){
    int pl=25000000;//0.25초
    int ph=pl>>16;
    *(TIMER_ptr+2)=pl;
    *(TIMER_ptr+3)=ph;
    *(TIMER_ptr+1)=0b0111;//stop,start,cont,ito=0111
}