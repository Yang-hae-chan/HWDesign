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

#define HEX3_HEX0_BASE			0xFF200020
#define HEX5_HEX4_BASE			0xFF200030
#define KEY_BASE				0xFF200050
volatile int *KEY_ptr = (int*)KEY_BASE;
volatile int *HEX3_HEX0_ptr = (int*)HEX3_HEX0_BASE;
unsigned int HEX_Output=0;
unsigned int HEX3=0,HEX2=0,HEX1=0,HEX0=0;
void enable_nios2_interrupts(void);
void config_KEYs(void);
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
void main(void);
void interrupt_handler(void);
void pushbutton_ISR(void);
/* The assembly language code below handles Nios II reset processing */
void the_reset (void) __attribute__ ((section (".reset")));
void the_reset (void)
/*******************************************************************************
* Reset code; by using the section attribute with the name ".reset" we allow the linker program
* to locate this code at the proper reset vector address. This code just calls the main program
******************************************************************************/
{
asm (".set noat"); // magic, for the C compiler
asm (".set nobreak"); // magic, for the C compiler
asm ("movia r2, main"); // call the C language main program
asm ("jmp r2");
}

/* The assembly language code below handles Nios II exception processing. This code should not be
* modified; instead, the C language code in the function interrupt_handler() can be modified as
* needed for a given application. */
void the_exception (void) __attribute__ ((section (".exceptions")));
void the_exception (void)
/*******************************************************************************
* Exceptions code; by giving the code a section attribute with the name ".exceptions" we allow
* the linker to locate this code at the proper exceptions vector address. This code calls the
* interrupt handler and later returns from the exception.
******************************************************************************/
{
asm (".set noat"); // magic, for the C compiler
asm (".set nobreak"); // magic, for the C compiler
asm ( "subi sp, sp, 128");
asm ( "stw et, 96(sp)");    //r24
asm ( "rdctl et, ctl4");    //et=ipendig
asm ( "beq et, r0, SKIP_EA_DEC"); // interrupt is not external(SW interrupt)
asm ( "subi ea, ea, 4"); /* must decrement ea by one instruction for external
* interrupts, so that the instruction will be run(HW interrupt이기 때문에 pc-4) */
asm ( "SKIP_EA_DEC:" );
asm (	"stw	r1,  4(sp)" );					// Save all registers 
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
asm (	"stw	r23, 92(sp)" );     // r24 = et, 앞에서 이미 store함
asm (	"stw	r25, 100(sp)" );	// r25 = bt (skip r24 = et, because it is saved above)
asm (	"stw	r26, 104(sp)" );	// r26 = gp
// skip r27 because it is sp, and there is no point in saving this
asm (	"stw	r28, 112(sp)" );	// r28 = fp
asm (	"stw	r29, 116(sp)" );	// r29 = ea
asm (	"stw	r30, 120(sp)" );	// r30 = ba
asm (	"stw	r31, 124(sp)" );	// r31 = ra
asm (	"addi	fp,  sp, 128" );
asm (   "call interrupt_handler" ); // call the C language interrupt handler

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
asm (	"ldw	r25, 100(sp)" );	// r25 = bt
asm (	"ldw	r26, 104(sp)" );	// r26 = gp
// skip r27 because it is sp, and we did not save this on the stack
asm (	"ldw	r28, 112(sp)" );	// r28 = fp
asm (	"ldw	r29, 116(sp)" );	// r29 = ea
asm (	"ldw	r30, 120(sp)" );	// r30 = ba
asm (	"ldw	r31, 124(sp)" );	// r31 = ra
asm (	"addi	sp,  sp, 128" );
asm (	"eret" );
}
/********************************************************************************
* Interrupt Service Routine: Determines the interrupt source and calls the appropriate subroutine
*******************************************************************************/
void interrupt_handler(void)
{
int ipending;
NIOS2_READ_IPENDING(ipending);
if ( ipending & 0x2 ) // pushbuttons are interrupt level 1
pushbutton_ISR( );
// else, ignore the interrupt
return;
}
unsigned int seg_code[] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111,
                            0b01100110, 0b01101101, 0b01111101, 0b00000111,
                            0b01111111, 0b01100111 };
void pushbutton_ISR(void){
    int KEY_edge=*(KEY_ptr+3);
    *(KEY_ptr+3)=KEY_edge;
    
    if(KEY_edge==0b1000){
        if(HEX3==0)
        HEX3=seg_code[3];
        else
        HEX3=0;        
    }
    else if(KEY_edge==0b0100){
        if(HEX2==0)
        HEX2=seg_code[2];
        else
        HEX2=0;        
    }
    else if(KEY_edge==0b0010){
        if(HEX1==0)
        HEX1=seg_code[1];
        else
        HEX1=0;        
    }
    else if(KEY_edge==0b0001){
        if(HEX0==0)
        HEX0=seg_code[0];
        else
        HEX0=0;        
    }
    HEX_Output=(HEX3<<24)|(HEX2<<16)|(HEX1<<8)|(HEX0);  
    return;
}