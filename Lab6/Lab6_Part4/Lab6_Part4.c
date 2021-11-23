#define HEX3_HEX0_BASE			0xFF200020
#define HEX5_HEX4_BASE			0xFF200030
#define TIMER_BASE				0xFF202000
#define KEY_BASE				0xFF200050
unsigned int seg_code[] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111,
                            0b01100110, 0b01101101, 0b01111101, 0b00000111,
                            0b01111111, 0b01100111 };
static volatile int *KEY_ptr = (int*)KEY_BASE;
static volatile int *TIMER_ptr = (int*)TIMER_BASE;
static volatile int *HEX3_HEX0_ptr = (int*)HEX3_HEX0_BASE;
static volatile int *HEX5_HEX4_ptr = (int*)HEX5_HEX4_BASE;
static volatile int *TIMER_SET = (int*)(TIMER_BASE + 4);
static volatile int *TIMER_pl = (int*)(TIMER_BASE + 8);
static volatile int *TIMER_ph = (int*)(TIMER_BASE + 12);

void TIMER_pl_ph(int ms) {
	int pl = ms * 100000; 	
	int ph = pl >> 16;  	
	*TIMER_pl = pl; 	
	*TIMER_ph = ph;//timer value 설정
}


char TIMER_TO() {// TO
	return (*TIMER_ptr) & 0b01;
}				
void CLEAR_TIMER_TO() {// TO초기화
	*TIMER_ptr = 0;
}				
void Timer_SET(int set) {// Timer setting
	*TIMER_SET = set;
}				

char READ_KEY() {// Key 눌렸는지
	return (*KEY_ptr);		
}

char READ_RUN() {//RUN 인지
	return (*TIMER_ptr) & 0b10;
}

// HEX3_2: sec, HEX1_0: msec
void DISPLAY_HEX3_0(int num) {
	unsigned int OS;
	unsigned int TS;
	num = num % 6000;//분단위 버림    	
	OS = seg_code[num / 1000];//10초단위
	TS = OS << 8;   		
	num = num % 1000;//10초단위 버림    	
	OS = seg_code[num / 100];//1초단위 	
	TS = TS | OS;//10초 1초 단위 합침   		
	TS = TS << 8; //HEX한칸씩 밀어냄    		
	num = num % 100;//초단위 버림    		
	OS = seg_code[num / 10];//0.1초단위 	
	TS = TS | OS;//0.1초 단위까지 합침   		
	TS = TS << 8;//HEX한칸씩 밀어냄   		
	OS = seg_code[num % 10];//0.01초단위	
	TS = TS | OS;//최종적으로 합침   		
	*HEX3_HEX0_ptr = TS;//HEX에 입력   	
}
//HEX5_4: min
void DISPLAY_HEX5_4(int num) {
	unsigned int TM;
	unsigned int OM;
	num = num / 6000;//분단위    		
	num = num % 60;   		
	TM = seg_code[num / 10]; //10분단위	 
	OM = seg_code[num % 10]; //1분단위	
	TM = TM << 8;//HEX5   	
	TM = (TM | OM);//합침 		
	*HEX5_HEX4_ptr = TM;
}

//main 함수
int main()
{
	unsigned int count = 0;	// COUNT 초기화

	TIMER_pl_ph(10);  		// timer value = 10ms 
	Timer_SET(0b0110);  	// timer start

	DISPLAY_HEX3_0(0);  	
	DISPLAY_HEX5_4(0);	    // HEX OFF


	while (1) {    		
		if (READ_RUN() && TIMER_TO()) {
			count++; // 1씩 카운트
			if (count == 6000 * 60) count = 0;// 한 시간이 되면 초기화
			DISPLAY_HEX3_0(count); // 시간 Display
			DISPLAY_HEX5_4(count);
			CLEAR_TIMER_TO();    // TO 초기화
		}
			while (READ_KEY()) { // Key 누름 확인
			while (READ_KEY()) { // Key 뗌 확인
			}
			if (READ_RUN()) Timer_SET(0b1010); // RUN이 1이면 STOP
			else Timer_SET(0b0110); // RUN이 0이면 START
		}
	}
}