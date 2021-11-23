#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"
#include <stdio.h>
#include "alu.c"
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

//표시할 스크린의 크기 상수 정의 
#define PIXEL(r,g,b) (short int)((((r)&0x1f)<<11)|(((g)&0x3f)<<5)|(((b)&0x1f)))
#define FILL_PIXEL(x,y,r,g,b) *(short int*)(pixel_buffer_start +(((y)&0xff)<<10)+ (((x)&0x1ff)<<1))=PIXEL(r,g,b)
volatile int pixel_buffer_start;//화면 저장
volatile int* pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;
//전역 번수 전언

/* these globals are written by interrupt service routines; we have to declare
 * these as volatile to avoid the compiler caching their values in registers */
extern volatile char byte1, byte2,byte3; // modified by PS/2 interrupt service routine
extern volatile int timeout; // used to synchronize with the timer
int xx=10;
int yy=0;
char ss;
char input[100];
int index=0;
char sc[3];
/* function prototypes */
void clear_screen(int r, int g, int b);
void HEX_PS2(char, char, char);
void write_pixel(int x, int y, short colour);
void clear_char();
void clear_char_all();
void write_char(int x, int y, char c);
void print();
/*******************************************************************************
 * This program performs the following:
 * Displays the last three bytes of data received from the PS/2 port
 * on the HEX displays. The PS/2 port is handled using interrupts
 ******************************************************************************/
int main(void) {
    pixel_buffer_start = *pixel_ctrl_ptr;
    /* Declare volatile pointers to I/O registers (volatile means that IO load
       and store instructions will be used to access these pointer locations,
       instead of regular memory loads and stores) */
    volatile int * interval_timer_ptr = (int *)TIMER_BASE;                    // interal timer base address
    volatile int * PS2_ptr = (int *)PS2_BASE; // PS/2 port address
    /* initialize some variables */
    byte1   = 0;
    byte2   = 0;
    byte3   = 0; // used to hold PS/2 data
    timeout = 0; // synchronize with the timer

    /* set the interval timer period for scrolling the HEX displays */
    int counter = 20000000; // 1/(100 MHz) x (20000000) = 200 msec
    *(interval_timer_ptr + 0x2) = (counter & 0xFFFF);
    *(interval_timer_ptr + 0x3) = (counter >> 16) & 0xFFFF;

    /* start interval timer, enable its interrupts */
    *(interval_timer_ptr + 1) = 0x7; // STOP = 0, START = 1, CONT = 1, ITO = 1

    *(PS2_ptr) = 0xFF; /* reset */
    *(PS2_ptr + 1) =
        0x1; /* write to the PS/2 Control register to enable interrupts */

    NIOS2_WRITE_IENABLE(
        0x81); /* set interrupt mask bits for levels 0 (interval
                * timer) and 7 (PS/2) */

    NIOS2_WRITE_STATUS(1); // enable Nios II interrupts
    clear_screen(0x00,0x00,0x00);
    while (1) {
        write_char(xx,yy, '|');
        while (!timeout){}
        xx+=2;clear_char();   
             // wait to synchronize with timer
        /* display PS/2 data (from interrupt service routine) on HEX displays */
        /*while(byte2!=0xF0){
            print();
        }*/
        if(byte3 !=0&&byte2!=0){
            HEX_PS2(byte1, byte2, byte3);
            
        }
        byte1=0; byte2=0; byte3=0;
        timeout = 0;
        while (!timeout){}
        timeout = 0;
    }
}

/*******************************************************************************
 * Subroutine to show a string of HEX data on the HEX displays
 ******************************************************************************/
void HEX_PS2(char b1, char b2, char b3) {
    volatile int* HEX3_HEX0_ptr = (int*)HEX3_HEX0_BASE;
    /* SEVEN_SEGMENT_DECODE_TABLE gives the on/off settings for all segments in
    * a single 7-seg display in the DE1-SoC Computer, for the hex digits 0 - F
    */
    unsigned int seg_code[] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111,
                            0b01100110, 0b01101101, 0b01111101, 0b00000111,
                            0b01111111, 0b01100111 };//
    int seg;
    char s;
    if (b3 == 0x16) {s = '1'; seg=1;input[index]='1';index++;}
    if (b3 == 0x1E) {s = '2'; seg=2;input[index]='2';index++;}
    if (b3 == 0x26) {s = '3'; seg=3;input[index]='3';index++;}
    if (b3 == 0x25) {s = '4'; seg=4;input[index]='4';index++;}
    if (b3 == 0x2E) {s = '5'; seg=5;input[index]='5';index++;}
    if (b3 == 0x36) {s = '6'; seg=6;input[index]='6';index++;}
    if (b3 == 0x3D) {s = '7'; seg=7;input[index]='7';index++;}
    //if (b3 == 0x3E) {s = '8'; seg=8;}
    if (b3 == 0x46) {s = '9'; seg=9;input[index]='9';index++;}
    if (b3 == 0x45) {s = '0'; seg=0;input[index]='0';index++;}
    if (b3 == 0x55) {s = '+';input[index]='+';index++;}
    if (b3 == 0x4E) {s = '-';input[index]='-';index++;}
    if (b3 == 0x3E) {s = '*';input[index]='*';index++;}
    if (b3 == 0x4A) {s = '/';input[index]='/';index++;}
    if (b3 == 0x21) {sc[0] ='c';sc[1] ='o';sc[2] ='c';}
    if (b3 == 0x1B) {sc[0] ='s';sc[1] ='i';sc[2] ='n';}
    if (b3 == 0x5A) {clear_char_all();printf(calc(input));index=0; return;}
    if (b3 == 0x66) {clear_char(); return;}
    ss=s;
    print();
    *(HEX3_HEX0_ptr) = seg_code[seg];
}
void print(){
    if(sc[0]=='s'){
        for (int i=0;i<3;i++) {
            write_char(xx, yy, sc[i]);
            xx++;
            sc[i]='\0';
        }
    }
    if(*sc=='c'){
        for (int i=0;i<3;i++) {
            write_char(xx, yy,sc[i]);
            xx++;
            sc[i]='\0';
        }
    }
    else {
        write_char(xx, yy, ss);
        xx+=2;
    }
}
void write_pixel(int x, int y, short colour) {
    volatile short* vga_addr = (volatile short*)(0x08000000 + (y << 10) + (x << 1));
    *vga_addr = colour;
}

/* use write_pixel to set entire screen to black (does not clear the character buffer) */
void clear_screen(int r, int g, int b){//인수로 받은 rgb값으로 clear
    int x, y;
    for(x=0;x<SCREEN_WIDTH+1;x++){
        for(y=0;y<SCREEN_HEIGHT+1;y++){
            FILL_PIXEL(x,y,r,g,b);
        }
    }
}
/* write a single character to the character buffer at x,y
 * x in [0,79], y in [0,59]
 */
void write_char(int x, int y, char c) {
    // VGA character buffer
    volatile char* character_buffer = (char*)(0x09000000 + (y << 8) + x);
    *character_buffer = c;
}
void clear_char(){
    xx-=2;
    volatile char* character_buffer = (char*)(0x09000000 + (yy << 8) + xx);
    *character_buffer = '\0';
}
void clear_char_all(){
    int i =50;
    int j =50;
    volatile char* character_buffer = (char*)(0x09000000);
    while(character_buffer!=(char*)FPGA_CHAR_END){
        *character_buffer = '\0';
        character_buffer++;
    }
    xx=10;
    yy=0;
    return;
}