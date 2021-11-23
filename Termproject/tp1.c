//main.c
#include <stdio.h>
#define DDR_BASE				0x40000000
#define DDR_END					0x7FFFFFFF
#define A9_ONCHIP_BASE			0xFFFF0000
#define A9_ONCHIP_END			0xFFFFFFFF
#define SDRAM_BASE				0x00000000
#define SDRAM_END				0x03FFFFFF
#define FPGA_ONCHIP_BASE		0x08000000
#define FPGA_ONCHIP_END			0x0803FFFF
#define FPGA_CHAR_BASE			0x09000000
#define FPGA_CHAR_END			0x09001FFF

/* Cyclone V FPGA devices */
#define LEDR_BASE				0xFF200000
#define HEX3_HEX0_BASE			0xFF200020
#define HEX5_HEX4_BASE			0xFF200030
#define SW_BASE					0xFF200040
#define KEY_BASE				0xFF200050
#define JP1_BASE				0xFF200060
#define JP2_BASE				0xFF200070
#define PS2_BASE				0xFF200100
#define PS2_DUAL_BASE			0xFF200108
#define JTAG_UART_BASE			0xFF201000
#define IrDA_BASE				0xFF201020
#define TIMER_BASE				0xFF202000
#define TIMER_2_BASE			0xFF202020
#define AV_CONFIG_BASE			0xFF203000
#define PIXEL_BUF_CTRL_BASE		0xFF203020
#define CHAR_BUF_CTRL_BASE		0xFF203030
#define AUDIO_BASE				0xFF203040
#define VIDEO_IN_BASE			0xFF203060
#define EDGE_DETECT_CTRL_BASE	0xFF203070
#define ADC_BASE				0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE			0xFF709000
#define I2C0_BASE				0xFFC04000
#define I2C1_BASE				0xFFC05000
#define I2C2_BASE				0xFFC06000
#define I2C3_BASE				0xFFC07000
#define HPS_TIMER0_BASE			0xFFC08000
#define HPS_TIMER1_BASE			0xFFC09000
#define HPS_TIMER2_BASE			0xFFD00000
#define HPS_TIMER3_BASE			0xFFD01000
#define FPGA_BRIDGE				0xFFD0501C
#define NIOS2_READ_IENABLE(dest) \
	do { dest = __builtin_rdctl(3); } while (0)

#define NIOS2_WRITE_IENABLE(src) \
	do { __builtin_wrctl(3, src); } while (0)

#define NIOS2_READ_IPENDING(dest) \
	do { dest = __builtin_rdctl(4); } while (0)

#define NIOS2_READ_STATUS(dest) \
	do { dest = __builtin_rdctl(0); } while (0)

#define NIOS2_WRITE_STATUS(src) \
	do { __builtin_wrctl(0, src); } while (0)
volatile int* PS2_ptr = (int*)0xFF200100;
void enable_nios2_interrupts(void); //interrupt를 enable하게 할 함수 선언
void config_PS2(void);// KEY초기 설정 함수
int xx=0;

int main(void) {

    config_PS2();// configure pushbutton KEYs to generate interrupts
    enable_nios2_interrupts(); // enable interrupts in the Nios II processor
	 
    while (1) {
        
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
int main(void);
void interrupt_handler(void);
void PS2_ISR(void);
//void timer_ISR(void);
/* The assembly language code below handles Nios II reset processing */
void the_reset(void) __attribute__((section(".reset")));
void the_reset(void)
{
    asm(".set noat"); // magic, for the C compiler
    asm(".set nobreak"); // magic, for the C compiler
    asm("movia r2, main"); // call the C language main program
    asm("jmp r2");
}

void the_exception(void) __attribute__((section(".exceptions")));
void the_exception(void)
{
    asm(".set noat"); // magic, for the C compiler
    asm(".set nobreak"); // magic, for the C compiler
    asm("subi sp, sp, 128");
    asm("stw et, 96(sp)");    //r24
    asm("rdctl et, ctl4");    //et=ipendig
    asm("beq et, r0, SKIP_EA_DEC"); // interrupt is not external(SW interrupt)
    asm("subi ea, ea, 4"); /* must decrement ea by one instruction for external
    * interrupts, so that the instruction will be run(HW interrupt이기 때문에 pc-4) */
    asm("SKIP_EA_DEC:");
    asm("stw	r1,  4(sp)");					// Save all registers 
    asm("stw	r2,  8(sp)");
    asm("stw	r3,  12(sp)");
    asm("stw	r4,  16(sp)");
    asm("stw	r5,  20(sp)");
    asm("stw	r6,  24(sp)");
    asm("stw	r7,  28(sp)");
    asm("stw	r8,  32(sp)");
    asm("stw	r9,  36(sp)");
    asm("stw	r10, 40(sp)");
    asm("stw	r11, 44(sp)");
    asm("stw	r12, 48(sp)");
    asm("stw	r13, 52(sp)");
    asm("stw	r14, 56(sp)");
    asm("stw	r15, 60(sp)");
    asm("stw	r16, 64(sp)");
    asm("stw	r17, 68(sp)");
    asm("stw	r18, 72(sp)");
    asm("stw	r19, 76(sp)");
    asm("stw	r20, 80(sp)");
    asm("stw	r21, 84(sp)");
    asm("stw	r22, 88(sp)");
    asm("stw	r23, 92(sp)");     // r24 = et, 앞에서 이미 store함
    asm("stw	r25, 100(sp)");	// r25 = bt (skip r24 = et, because it is saved above)
    asm("stw	r26, 104(sp)");	// r26 = gp
    // skip r27 because it is sp, and there is no point in saving this
    asm("stw	r28, 112(sp)");	// r28 = fp
    asm("stw	r29, 116(sp)");	// r29 = ea
    asm("stw	r30, 120(sp)");	// r30 = ba
    asm("stw	r31, 124(sp)");	// r31 = ra
    asm("addi	fp,  sp, 128");
    asm("call interrupt_handler"); // call the C language interrupt handler

    asm("ldw	r1,  4(sp)");			// Restore all registers
    asm("ldw	r2,  8(sp)");
    asm("ldw	r3,  12(sp)");
    asm("ldw	r4,  16(sp)");
    asm("ldw	r5,  20(sp)");
    asm("ldw	r6,  24(sp)");
    asm("ldw	r7,  28(sp)");
    asm("ldw	r8,  32(sp)");
    asm("ldw	r9,  36(sp)");
    asm("ldw	r10, 40(sp)");
    asm("ldw	r11, 44(sp)");
    asm("ldw	r12, 48(sp)");
    asm("ldw	r13, 52(sp)");
    asm("ldw	r14, 56(sp)");
    asm("ldw	r15, 60(sp)");
    asm("ldw	r16, 64(sp)");
    asm("ldw	r17, 68(sp)");
    asm("ldw	r18, 72(sp)");
    asm("ldw	r19, 76(sp)");
    asm("ldw	r20, 80(sp)");
    asm("ldw	r21, 84(sp)");
    asm("ldw	r22, 88(sp)");
    asm("ldw	r23, 92(sp)");
    asm("ldw	r24, 96(sp)");
    asm("ldw	r25, 100(sp)");	// r25 = bt
    asm("ldw	r26, 104(sp)");	// r26 = gp
    // skip r27 because it is sp, and we did not save this on the stack
    asm("ldw	r28, 112(sp)");	// r28 = fp
    asm("ldw	r29, 116(sp)");	// r29 = ea
    asm("ldw	r30, 120(sp)");	// r30 = ba
    asm("ldw	r31, 124(sp)");	// r31 = ra
    asm("addi	sp,  sp, 128");
    asm("eret");
}
/********************************************************************************
* Interrupt Service Routine: Determines the interrupt source and calls the appropriate subroutine
*******************************************************************************/
void interrupt_handler(void)
{
    int ipending;
    NIOS2_READ_IPENDING(ipending);
    if (ipending & 0b010000000) // pushbuttons are interrupt level 1
        PS2_ISR();//pushbutton에 의한 interrupt일 때
        /*else if( ipending & 0x1 )
        timer_ISR();//TIMER에 의한 interrupt일 때
        // else, ignore the interrupt*/
    return;
}
extern volatile int* PS2_ptr;    //main code에서 사용한 전역변수
void clear_screen();
void write_char(int x, int y, char c);
void write_pixel(int x, int y, short colour);
void HEX_PS2(int b3);
void PS2_ISR(void);
void PS2_ISR( void )
{
	char byte1=0,byte2=0,byte3=3;
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
		if(byte3==0xF0) return;
		HEX_PS2(byte3);
	}
	return;
}

void HEX_PS2(int b3) {
    volatile int* HEX3_HEX0_ptr = (int*)HEX3_HEX0_BASE;
    /* SEVEN_SEGMENT_DECODE_TABLE gives the on/off settings for all segments in
    * a single 7-seg display in the DE1-SoC Computer, for the hex digits 0 - F
    */
    unsigned int seg_code[] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111,
                            0b01100110, 0b01101101, 0b01111101, 0b00000111,
                            0b01111111, 0b01100111 };//
    int seg;
    char s;
	if (b3== 0xF0) {s='\0';*(HEX3_HEX0_ptr) =0; }
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
void write_pixel(int x, int y, short colour) {
    volatile short* vga_addr = (volatile short*)(0x08000000 + (y << 10) + (x << 1));
    *vga_addr = colour;
}

/* use write_pixel to set entire screen to black (does not clear the character buffer) */
void clear_screen() {
    int x, y;
    for (x = 0; x < 320; x++) {
        for (y = 0; y < 240; y++) {
            write_pixel(x, y, 0);
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