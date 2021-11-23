/*******************************************************************************
 * This file provides address values that exist in the DE1-SoC Computer
 ******************************************************************************/

#ifndef __SYSTEM_INFO__
#define __SYSTEM_INFO__

#define BOARD				"DE1-SoC"

/* Memory */
#define DDR_BASE			0x40000000
#define DDR_END				0x7FFFFFFF
#define A9_ONCHIP_BASE			0xFFFF0000
#define A9_ONCHIP_END			0xFFFFFFFF
#define SDRAM_BASE			0x00000000
#define SDRAM_END			0x03FFFFFF
#define FPGA_PIXEL_BUF_BASE		0x08000000
#define FPGA_PIXEL_BUF_END		0x0803FFFF
#define FPGA_CHAR_BASE			0x09000000
#define FPGA_CHAR_END			0x09001FFF

/* Cyclone V FPGA devices */
#define LED_BASE			0xFF200000
#define LEDR_BASE			0xFF200000
#define HEX3_HEX0_BASE			0xFF200020
#define HEX5_HEX4_BASE			0xFF200030
#define SW_BASE				0xFF200040
#define KEY_BASE			0xFF200050
#define JP1_BASE			0xFF200060
#define JP2_BASE			0xFF200070
#define PS2_BASE			0xFF200100
#define PS2_DUAL_BASE			0xFF200108
#define JTAG_UART_BASE			0xFF201000
#define IrDA_BASE			0xFF201020
#define TIMER_BASE			0xFF202000
#define TIMER_2_BASE			0xFF202020
#define AV_CONFIG_BASE			0xFF203000
#define RGB_RESAMPLER_BASE    		0xFF203010
#define PIXEL_BUF_CTRL_BASE		0xFF203020
#define CHAR_BUF_CTRL_BASE		0xFF203030
#define AUDIO_BASE			0xFF203040
#define VIDEO_IN_BASE			0xFF203060
#define EDGE_DETECT_CTRL_BASE		0xFF203070
#define ADC_BASE			0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE			0xFF709000
#define I2C0_BASE			0xFFC04000
#define I2C1_BASE			0xFFC05000
#define I2C2_BASE			0xFFC06000
#define I2C3_BASE			0xFFC07000
#define HPS_TIMER0_BASE			0xFFC08000
#define HPS_TIMER1_BASE			0xFFC09000
#define HPS_TIMER2_BASE			0xFFD00000
#define HPS_TIMER3_BASE			0xFFD01000
#define FPGA_BRIDGE			0xFFD0501C

#endif
#ifndef __NIOS2_CTRL_REG_MACROS__
#define __NIOS2_CTRL_REG_MACROS__

/*****************************************************************************/
/* Macros for accessing the control registers.                               */
/*****************************************************************************/

#define NIOS2_READ_STATUS(dest) \
	do { dest = __builtin_rdctl(0); } while (0)

#define NIOS2_WRITE_STATUS(src) \
	do { __builtin_wrctl(0, src); } while (0)

#define NIOS2_READ_ESTATUS(dest) \
	do { dest = __builtin_rdctl(1); } while (0)

#define NIOS2_READ_BSTATUS(dest) \
	do { dest = __builtin_rdctl(2); } while (0)

#define NIOS2_READ_IENABLE(dest) \
	do { dest = __builtin_rdctl(3); } while (0)

#define NIOS2_WRITE_IENABLE(src) \
	do { __builtin_wrctl(3, src); } while (0)

#define NIOS2_READ_IPENDING(dest) \
	do { dest = __builtin_rdctl(4); } while (0)

#define NIOS2_READ_CPUID(dest) \
	do { dest = __builtin_rdctl(5); } while (0)

#endif
volatile char byte1, byte2, byte3; // PS/2 variables

volatile int timeout; // used to synchronize with the timer

#include <stdio.h>
#define MAX 100
int stack[MAX];
int top;
void init_stack(void) {
    top = -1;
}
int is_operator(int k) {
    return (k == '+' || k == '-' ||
        k == '*' || k == '/');
}
void print_stack(void) {
    int i;
    printf("\n Stack contents : Top --> Bottom\n");
    for (i = top; i >= 0; i--)
        printf("%-6c", stack[i]);
}
int pop(void) {
    if (top < 0) {
        printf("\n Stack underflow.");
        return -1;
    }
    return stack[top--];
}
int push(int t) {
    if (top >= MAX - 1) {
        printf("\n Stack overflow.");
        return -1;
    }
    stack[++top] = t;
    return t;
}

int precedence(int op) {
    if (op == '(') return 0;
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    else return 3;
}
int get_stack_top(void) {
    return (top < 0) ? -1 : stack[top];
}
int is_stack_empty(void) {
    return (top < 0);
}

void postfix(char* dst, char* src) {
    char c;
    init_stack();
    while (*src) {
        if (*src == '(') { push(*src); src++; }
        else if (*src == ')') {
            while (get_stack_top() != '(') {
                *dst++ = pop(); *dst++ = ' ';
                printf("post: %c", *src);
            }
            pop(); src++;
            print_stack(); printf("\n");
        }
        else if (is_operator(*src)) {
            while (!is_stack_empty() &&
                precedence(get_stack_top()) >=
                precedence(*src)) {
                *dst++ = pop(); *dst++ = ' ';
                printf("post: %c", *src);
            }
            push(*src); src++;
            print_stack(); printf("\n");
        }
        else if (*src >= '0' && *src <= '9') {
            do {printf("post: %c", *src);
                *dst++ = *src++;
            } while (*src >= '0' && *src <= '9');
            
            *dst++ = ' ';
        }
        else src++;
    }
    while (!is_stack_empty()) {
        *dst++ = pop(); *dst++ = ' ';
    }
    dst--; *dst = 0;
}
int calc(char* p) {
    int i;
    init_stack();
    while (*p) {
        if (*p >= '0' && *p <= '9') {
            i = 0;
            do {
                i = i * 10 + *p - '0';
                p++;
            } while (*p >= '0' && *p <= '9');
            push(i);
        }
        else if (*p == '+') {
            push(pop() + pop());
            p++;
        }
        else if (*p == '*') {
            push(pop() * pop());
            p++;
        }
        else if (*p == '-') {
            i = pop();
            push(pop() - i);
            p++;
        }
        else if (*p == '/') {
            i = pop();
            push(pop() / i);
            p++;
        }
        else p++;
    }
    return pop();
}
#include <stdio.h>
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
void main(void) {
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
/* function prototypes */
void main(void);
void interrupt_handler(void);
void interval_timer_ISR(void);
void PS2_ISR(void);

/* The assembly language code below handles CPU reset processing */
void the_reset (void) __attribute__ ((section (".reset")));
void the_reset (void)
/*******************************************************************************
 * Reset code. By giving the code a section attribute with the name ".reset" we
 * allow the linker program to locate this code at the proper reset vector
 * address. This code just calls the main program.
 ******************************************************************************/
{
asm (".set		noat");					// Magic, for the C compiler
asm (".set		nobreak");				// Magic, for the C compiler
asm ("movia		r2, main");				// Call the C language main program
asm ("jmp		r2"); 
}

/* The assembly language code below handles CPU exception processing. This
 * code should not be modified; instead, the C language code in the function
 * interrupt_handler() can be modified as needed for a given application.
*/
void the_exception (void) __attribute__ ((section (".exceptions")));
void the_exception (void)
/*******************************************************************************
 * Exceptions code. By giving the code a section attribute with the name
 * ".exceptions" we allow the linker program to locate this code at the proper
 * exceptions vector address.
 * This code calls the interrupt handler and later returns from the exception.
 ******************************************************************************/
{
asm ( ".set		noat" );				// Magic, for the C compiler
asm ( ".set		nobreak" );				// Magic, for the C compiler
asm ( "subi		sp, sp, 128" );
asm ( "stw		et, 96(sp)" );
asm ( "rdctl	et, ctl4" );
asm ( "beq		et, r0, SKIP_EA_DEC" );	// Interrupt is not external         
asm ( "subi		ea, ea, 4" );			/* Must decrement ea by one instruction  
									 	 * for external interupts, so that the 
									 	 * interrupted instruction will be run */

asm ( "SKIP_EA_DEC:" );
asm (	"stw	r1,  4(sp)" );			// Save all registers 
asm (	"stw	r2,  8(sp)" );
asm (	"stw	r3,  12(sp)" );
asm (	"stw	r4,  16(sp)" );
asm (	"stw	r5,  20(sp)" );
asm (	"stw	r6,  24(sp)" );
asm (	"stw	r7,  28(sp)" );
asm (	"stw	r8,  32(sp)" );
asm (	"stw	r9,  36(sp)" );
asm (	"stw	r10, 40(sp)" );
asm (	"stw	r11, 44(sp)" );
asm (	"stw	r12, 48(sp)" );
asm (	"stw	r13, 52(sp)" );
asm (	"stw	r14, 56(sp)" );
asm (	"stw	r15, 60(sp)" );
asm (	"stw	r16, 64(sp)" );
asm (	"stw	r17, 68(sp)" );
asm (	"stw	r18, 72(sp)" );
asm (	"stw	r19, 76(sp)" );
asm (	"stw	r20, 80(sp)" );
asm (	"stw	r21, 84(sp)" );
asm (	"stw	r22, 88(sp)" );
asm (	"stw	r23, 92(sp)" );
asm (	"stw	r25, 100(sp)" );		// r25 = bt (skip r24 = et, because it is saved above)
asm (	"stw	r26, 104(sp)" );		// r26 = gp
// skip r27 because it is sp, and there is no point in saving this
asm (	"stw	r28, 112(sp)" );		// r28 = fp
asm (	"stw	r29, 116(sp)" );		// r29 = ea
asm (	"stw	r30, 120(sp)" );		// r30 = ba
asm (	"stw	r31, 124(sp)" );		// r31 = ra
asm (	"addi	fp,  sp, 128" );

asm (	"call	interrupt_handler" );	// Call the C language interrupt handler

asm (	"ldw	r1,  4(sp)" );			// Restore all registers
asm (	"ldw	r2,  8(sp)" );
asm (	"ldw	r3,  12(sp)" );
asm (	"ldw	r4,  16(sp)" );
asm (	"ldw	r5,  20(sp)" );
asm (	"ldw	r6,  24(sp)" );
asm (	"ldw	r7,  28(sp)" );
asm (	"ldw	r8,  32(sp)" );
asm (	"ldw	r9,  36(sp)" );
asm (	"ldw	r10, 40(sp)" );
asm (	"ldw	r11, 44(sp)" );
asm (	"ldw	r12, 48(sp)" );
asm (	"ldw	r13, 52(sp)" );
asm (	"ldw	r14, 56(sp)" );
asm (	"ldw	r15, 60(sp)" );
asm (	"ldw	r16, 64(sp)" );
asm (	"ldw	r17, 68(sp)" );
asm (	"ldw	r18, 72(sp)" );
asm (	"ldw	r19, 76(sp)" );
asm (	"ldw	r20, 80(sp)" );
asm (	"ldw	r21, 84(sp)" );
asm (	"ldw	r22, 88(sp)" );
asm (	"ldw	r23, 92(sp)" );
asm (	"ldw	r24, 96(sp)" );
asm (	"ldw	r25, 100(sp)" );		// r25 = bt
asm (	"ldw	r26, 104(sp)" );		// r26 = gp
// skip r27 because it is sp, and we did not save this on the stack
asm (	"ldw	r28, 112(sp)" );		// r28 = fp
asm (	"ldw	r29, 116(sp)" );		// r29 = ea
asm (	"ldw	r30, 120(sp)" );		// r30 = ba
asm (	"ldw	r31, 124(sp)" );		// r31 = ra

asm (	"addi	sp,  sp, 128" );

asm (	"eret" );
}

/******************************************************************************
 * Interrupt Service Routine                                                 
 *  Determines what caused the interrupt and calls the appropriate          
 *  subroutine.                                                              
 *                                                                           
 * ipending - Control register 4 which has the pending external interrupts  
 ******************************************************************************/
void interrupt_handler(void)
{
	int ipending;
	NIOS2_READ_IPENDING(ipending);
	if ( ipending & 0x01 )	// interval timer is interrupt level 0
	{
		interval_timer_ISR( );
	}
	if ( ipending & 0x80)	// PS/2 port is interrupt level 7
	{
		PS2_ISR( );
	}
	// else, ignore the interrupt
	return;
}

extern volatile int timeout;

/*****************************************************************************
 * Interval timer interrupt service routine
 *                                                                          
 * Controls refresh of the VGA screen
 * 
******************************************************************************/
void interval_timer_ISR( )
{
	volatile int * interval_timer_ptr = (int *) TIMER_BASE;

	*(interval_timer_ptr) = 0; 						// clear the interrupt
	timeout = 1;									// set global variable
	return;
}


extern volatile char byte1, byte2, byte3;

/*******************************************************************************
 * PS2 - Interrupt Service Routine                                
 *                                                                          
 * This routine stores to last three bytes received from the PS/2 device.
 ******************************************************************************/
void PS2_ISR( void )
{
  	volatile int * PS2_ptr = (int *) PS2_BASE;		// PS/2 port address
	int PS2_data, RAVAIL;

	PS2_data = *(PS2_ptr);							// read the Data register in the PS/2 port
	RAVAIL = (PS2_data & 0xFFFF0000) >> 16;			// extract the RAVAIL field
	if (RAVAIL > 0)
	{
		/* always save the last three bytes received */
		byte1 = byte2;
		byte2 = byte3;
		byte3 = PS2_data & 0xFF;
		if ( (byte2 == (char) 0xAA) && (byte3 == (char) 0x00) )
			// mouse inserted; initialize sending of data
			*(PS2_ptr) = 0xF4;
	}
	return;
}
