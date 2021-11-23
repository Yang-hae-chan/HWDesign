#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"
#include <stdio.h>
#include <math.h>
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define pi 3.141592
//표시할 스크린의 크기 상수 정의 
#define PIXEL(r,g,b) (short int)((((r)&0x1f)<<11)|(((g)&0x3f)<<5)|(((b)&0x1f)))
#define FILL_PIXEL(x,y,r,g,b) *(short int*)(pixel_buffer_start +(((y)&0xff)<<10)+ (((x)&0x1ff)<<1))=PIXEL(r,g,b)
volatile int pixel_buffer_start;//화면 저장
volatile int* pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;
//전역 번수 전언
#define MAX 100
int aa=1;
volatile int stack[MAX];
int top;
/* these globals are written by interrupt service routines; we have to declare
 * these as volatile to avoid the compiler caching their values in registers */
extern volatile char byte1, byte2,byte3; // modified by PS/2 interrupt service routine
extern volatile int timeout; // used to synchronize with the timer
int xx=10;
int yy=0;
char ss;
volatile char input[100];
volatile char output[100];
int index=-1;
/* function prototypes */
int ROUND(float in);
void write_poly();
void draw_poly(int r, int g, int b);
void draw_line(int x1, int y1, int x2, int y2, int r, int g, int b);
float COS(float x, int num);
int POW(int n, int num);
int FACT(int n);
float rad(int x);
void print_postfix(void);
void clear_screen(int r, int g, int b);
void HEX_PS2(char, char, char);
void write_pixel(int x, int y, short colour);
void clear_char();
void clear_char_all();
void write_char(int x, int y, char c);
void print();
void clear_input();
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
        xx++ ;  clear_char();
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
float COS(float x, int num) {
	if (num <= 0) return 1;
	return POW(-1, num)*POW(x, 2 * num) / FACT(2 * num) + COS(x, num - 1);
}
float radian(int x) {
	x = x % 360;
	return (float)pi*(float)x / 180;
}

int POW(int n, int num){
    int out=1;
    for(int i=0;i<num;i++){
        out*=n;
    }
    return n;
}
int FACT(int n){
    int out=1;
    for(int i=1;i<=n;i++){
        out*=i;
    }
    return out;
}
int ROUND(float in){
    if(in>0){
        if((in-(int)in)<0.5) return (int)in;
        else return (int)in+1;
    }
    else{
        if(((int)in-in)<0.5) return (int)in;
        else return (int)in-1;        
    }
}
void draw_line(int x1, int y1, int x2, int y2, int r, int g, int b) {
    //선을 그리는 함수, 강의자료에 있는 알고리즘 사용함 설명 생략
    int tmp, x, y;
    int grad = (abs(x2 - x1) < abs(y2 - y1));
    if (grad) {
        tmp = x2;
        x2 = y2;
        y2 = tmp;
        tmp = x1;
        x1 = y1;
        y1 = tmp;
    }
    if (x1 > x2) {
        tmp = x1;
        x1 = x2;
        x2 = tmp;
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int err = -(dx / 2);
    y = y1;
    int y_step;
    if (y1 < y2) y_step = 1;
    else y_step = -1;
    for (x = x1;x <= x2;x++) {
        if (grad) {
            FILL_PIXEL(y, x, r, g, b);
        }
        else FILL_PIXEL(x, y, r, g, b);
        err = err + dy;
        if (err >= 0) {
            y += y_step;
            err -= dx;
        }
    }
}
void write_poly(){
    write_char(xx++,yy,'y');
    write_char(xx++,yy,'=');
    if(input[0]!='0'){
        write_char(xx++,yy,input[0]);
        write_char(xx++,yy,'x');
        write_char(xx++,yy,'^');
        write_char(xx++,yy,'3');
    }
    if(input[1]!='0'){
        write_char(xx++,yy,'+');
        write_char(xx++,yy,input[1]);
        write_char(xx++,yy,'x');
        write_char(xx++,yy,'^');
        write_char(xx++,yy,'2');
    }
    if(input[2]!='0'){
        write_char(xx++,yy,'+');
        write_char(xx++,yy,input[2]);
        write_char(xx++,yy,'x');
    }
    if(input[3]!='0'){
        write_char(xx++,yy,'+');
        write_char(xx++,yy,input[3]);
    }
    return;
}
void draw_poly(int r, int g, int b){
    clear_screen(0,0,0);
    draw_line(50,20,250,20,0xFF,0xFF,0xFF);
    draw_line(50,220,250,220,0xFF,0xFF,0xFF);
    draw_line(50,20,50,220,0xFF,0xFF,0xFF);
    draw_line(250,20,250,220,0xFF,0xFF,0xFF);
    draw_line(50,120,250,120,0x0,0xF,0xF);
    draw_line(150,20,150,220,0x0,0xF,0xF);
    int k=0;
    for(k=20;k<220;k+=5){
        FILL_PIXEL(150,k,0xFF,0,0);
        k+5;
    }
    for(k=0;k<100;k+=(5*aa)){
        FILL_PIXEL(150-k,120,0xFF,0,0);
        FILL_PIXEL(150+k,120,0xFF,0,0);
    }
    for(int i=-150;i<150;i++){
        int y1=ROUND(((float)(input[0]-48)*i*i*i/(aa*aa*aa)+(float)(input[1]-48)*i*i/(aa*aa)+(float)(input[2]-48)*i/aa+(float)(input[3]-48)));
        int y2=ROUND(((float)(input[0]-48)*(i+1)*(i+1)*(i+1)/(aa*aa*aa)+(float)(input[1]-48)*(i+1)*(i+1)/(aa*aa)+(float)(input[2]-48)*(i+1)/aa+(float)(input[3]-48)));
        if((120-y1)<220&&(120-y1)>20&&(120-y2)<220&&(120-y2)>20&&(i+150)>50&&(i+150)<250){
            draw_line(i+150,120-y1,i+151,120-y2,r,g,b);
        }
    }
}
/*void draw_poly(float input, int r, int g, int b){
    for(int i=0;i<300;i++){
        FILL_PIXEL(i,120-(int)(COS(i*pi,40)),r,g,b);
    }
}*/
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
    if (b3 == 0x69) {s = '1'; seg=1;input[++index]='1';}
    if (b3 == 0x72) {s = '2'; seg=2;input[++index]='2';}
    if (b3 == 0x7A) {s = '3'; seg=3;input[++index]='3';}
    if (b3 == 0x6B) {s = '4'; seg=4;input[++index]='4';}
    if (b3 == 0x73) {s = '5'; seg=5;input[++index]='5';}
    if (b3 == 0x74) {s = '6'; seg=6;input[++index]='6';}
    if (b3 == 0x6C) {s = '7'; seg=7;input[++index]='7';}
    if (b3 == 0x75) {s = '8'; seg=8;}
    if (b3 == 0x7D) {s = '9'; seg=9;input[++index]='9';}
    if (b3 == 0x70) {s = '0'; seg=0;input[++index]='0';}
    if (b3 == 0x55) {s = '+';input[++index]='+';}
    if (b3 == 0x4E) {s = '-';input[++index]='-';}
    if (b3 == 0x3E) {s = '*';input[++index]='*';}
    if (b3 == 0x4A) {s = '/';input[++index]='/';}
    if (b3 == 0x46) {s = '(';input[++index]='(';}
    if (b3 == 0x45) {s = ')';input[++index]=')';}
    if (b3 == 0x29) {write_poly(); draw_poly(0x00,0x00,0xFF); return;}
    if (b3 == 0x79) {aa++; draw_poly(0x00,0x00,0xFF); return;}
    if (b3 == 0x7B) {aa--; draw_poly(0x00,0x00,0xFF); return;}
    if (b3 == 0x76) {clear_char_all();clear_input(); clear_screen(0,0,0); return;}
    if (b3 == 0x66) {clear_char(); input[index]='\0';index--;; return;} 
    if (b3 == 0x41) {index--;xx--; return;}
    if (b3 == 0x49) {index++;xx--; return;}               
    if (b3 == 0x5A) {ss='=';print();
                    postfix(output,input);print_postfix();
                    print_stack();
                    printf("\nans: %d",calc(output));
                    ss= (char)(calc(output)) + 0x30; print();
                    index=-1; return;}
    ss=s;
    print();
    *(HEX3_HEX0_ptr) = seg_code[seg];
}
void print(){
    write_char(xx, yy, ss);
    xx++;
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
    xx--;
    volatile char* character_buffer = (char*)(0x09000000 + (yy << 8) + xx);
    *character_buffer = '\0';
}
void clear_char_all(){
    volatile char* character_buffer = (char*)(0x09000000);
    while(character_buffer!=(char*)FPGA_CHAR_END){
        *character_buffer = '\0';
        character_buffer++;
    }
    xx=10;
    yy=0;
    return;
}
void print_postfix(void) {
    int i;
    printf("\n postfix:\n");
    for (i = 0; i <= 10; i++)
        printf("%c", output[i]);
}
void clear_input(void){
    for(index;index>=0;index--){
        input[index]='\0';
        output[index]='\0';
    }
    index=-1;
}