//main.c
#include <stdio.h>
#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"
volatile char byte3; // modified by PS/2 interrupt service routine
volatile int timeout;
void HEX_PS2(char b3);
void enable_nios2_interrupts(void); //interrupt를 enable하게 할 함수 선언
void config_PS2(void);// KEY초기 설정 함수
volatile int* PS2_ptr = (int*)0xFF200100;
int main(void) {
    volatile int * interval_timer_ptr = (int *)TIMER_BASE;
    volatile int* PS2_ptr = (int*)0xFF200100;
    /* initialize some variables */
    //byte1   = 0;
    //byte2   = 0;
    byte3   = 0; // used to hold PS/2 data
    timeout = 0; // synchronize with the timer
    /* set the interval timer period for scrolling the HEX displays */
    int counter = 20000000; // 1/(100 MHz) x (20000000) = 200 msec
    *(interval_timer_ptr + 0x2) = (counter & 0xFFFF);
    *(interval_timer_ptr + 0x3) = (counter >> 16) & 0xFFFF;
     /* start interval timer, enable its interrupts */
    *(interval_timer_ptr + 1) = 0x7; // STOP = 0, START = 1, CONT = 1, ITO = 1 
    config_PS2();// configure pushbutton KEYs to generate interrupts
    enable_nios2_interrupts(); // enable interrupts in the Nios II processor
    while (1) {
        while (!timeout)
            ; // wait to synchronize with timer
        /* display PS/2 data (from interrupt service routine) on HEX displays */
        HEX_PS2(byte3);
        timeout = 0;
    }
} // wait for an interrupt

void config_PS2() {
    *(PS2_ptr) = 0xFF; /* reset */
    *(PS2_ptr + 1) = 0x1; /* write to the PS/2 Control register to enable interrupts */
}
/* Enable interrupts in the Nios II processor */
void enable_nios2_interrupts(void) {
    NIOS2_WRITE_IENABLE(0b010000000);   //IRQ pushbutton
    NIOS2_WRITE_STATUS(0b01);      //interrupt.PIE=1
}
void HEX_PS2(char b3) {
    volatile int* HEX3_HEX0_ptr = (int*)HEX3_HEX0_BASE;
    /* SEVEN_SEGMENT_DECODE_TABLE gives the on/off settings for all segments in
    * a single 7-seg display in the DE1-SoC Computer, for the hex digits 0 - F
    */
    unsigned int seg_code[] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111,
                            0b01100110, 0b01101101, 0b01111101, 0b00000111,
                            0b01111111, 0b01100111 };//
    int seg;
    char s;
	if (b3== 0xF0) {s='\0'; }
    if (b3 == 0x16) {s = '1'; seg=1;}
    if (b3 == 0x1E) {s = '2'; seg=2;}
    if (b3 == 0x26) {s = '3'; seg=3;}
    if (b3 == 0x25) {s = '4'; seg=4;}
    if (b3 == 0x2E) {s = '5'; seg=5;}
    if (b3 == 0x36) {s = '6'; seg=6;}
    if (b3 == 0x3D) {s = '7'; seg=7;}
    if (b3 == 0x3E) {s = '8'; seg=8;}
    if (b3 == 0x46) {s = '9'; seg=9;}
    if (b3 == 0x45) {s = '0'; seg=0;}
    //if (b3 == 0x5A) {clear_screen();}
    printf(" %c ",s);
    *(HEX3_HEX0_ptr) = seg_code[seg];
}