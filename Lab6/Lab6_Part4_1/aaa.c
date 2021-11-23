#define HEX3_HEX0_BASE			0xFF200020
#define HEX5_HEX4_BASE			0xFF200030
#define TIMER_BASE				0xFF202000
#define KEY_BASE				0xFF200050

static volatile int *KEY_ptr = (int*)KEY_BASE;
static volatile int *TIMER_ptr = (int*)TIMER_BASE;
static volatile int *HEX3_HEX0_ptr = (int*)HEX3_HEX0_BASE;
static volatile int *HEX5_HEX4_ptr = (int*)HEX5_HEX4_BASE;
static volatile int *TIMER_ctrl = (int*)(TIMER_BASE + 4);
static volatile int *TIMER_periodl = (int*)(TIMER_BASE + 8);
static volatile int *TIMER_periodh = (int*)(TIMER_BASE + 12);

//ms단위로 TimerValue를 설정한다.
void TimerValue(int ms) {
	int tl = ms * 100000; 	// 1ms*100000 = 1/100MHz
	int th = tl >> 16;  	// th =  16bit (high)
	*TIMER_periodl = tl; 	// 각각 register에 저장
	*TIMER_periodh = th;
}

void TimerCtrl(int j) {
	*TIMER_ctrl = j;
}				// Timer ctl bit 설정

char Key() {
	return (*KEY_ptr);		// Key bit를 read
}

char Run() {
	return (*TIMER_ptr) & 0b10;
}				// RUN bit를 read

char TO() {
	return (*TIMER_ptr) & 0b01;
}				// TO(Time Out) bit를 read
void reTO() {
	*TIMER_ptr = 0;
}				// TO(Time Out) bit를 초기화
int seg_ment(int i) {
	char seg[10] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
	0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111 };
	return seg[i];
}			// HEX에 표시하기 위해 입력에 해당하는 비트열을 리턴


// HEX3_2에 초, HEX1_0에 0.01초 단위 표시
void Display_Hex3_0(int k) {
	unsigned int s1;
	unsigned int s0;

	k = k % 6000;    	// 분 단위를 생략(초만 표시할거니까)
	s1 = seg_ment(k / 1000); 	// s1 = 10초 자리의 segment 표시
	s0 = s1 << 8;   		// s1 8bit right shift하여 s0에 대입

	k = k % 1000;    	// 10초 단위를 생략
	s1 = seg_ment(k / 100); 	// st = 1초 자리의 segment 표시
	s0 = s0 | s1;   		// s0와 합쳐서
	s0 = s0 << 8;   		// 8bit right shift

	k = k % 100;    		// 1초 단위를 생략
	s1 = seg_ment(k / 10); 	// s1 = 0.1초 자리의 segment 표시
	s0 = s0 | s1;   		// s0와 합쳐서
	s0 = s0 << 8;   		// 8bit right shift

	s1 = seg_ment(k % 10);	// s1 = 0.01초 자리의 segment 표시
	s0 = s0 | s1;   		// s0와 합하여(OR)
	*HEX3_HEX0_ptr = s0;   	// HEX3_HEX0에 출력
}
//HEX5에 10분, HEX4에 1분 단위 표시
void Display_Hex5_4(int l) {
	unsigned int m1;
	unsigned int m0;
	unsigned int m2;

	l = l / 6000;    		// 시간단위를 생략
	l = l % 60;    		// 분 단위 밑으로 생략
	m1 = seg_ment(l / 10); 	// 1분 자리 segment 표시 
	m0 = seg_ment(l % 10); 	// 10분 자리 segment 표시

	m1 = m1 << 8;   	// 10분자리의 segment 표시를 8bit rightshift

	m2 = (m1 | m0); 		// 합하여(OR)
	*HEX5_HEX4_ptr = m2; 	// HEX5_HEX4에 출력
}

//main 함수
void main()
{
	unsigned int count = 0;	// 카운트 초기화

	TimerValue(10);  		// timer start value = 10ms 
	TimerCtrl(0b0110);  	// timer CONTINUE <- 1

	Display_Hex3_0(0);  	// 7 Segment
	Display_Hex5_4(0);	// 0으로 초기화


	while (1) {    		// 무한 반복
		if (Run() && TO()) {
			count++; // 1씩 카운트
			if (count == 6000 * 60) count = 0;// 한 시간이 되면 초기화
			Display_Hex3_0(count); // 시간 Display
			Display_Hex5_4(count);
			reTO();    // TO 초기화
		}
			while (Key()) { // Key 누름 확인
			while (Key()) { // Key 뗌 확인
			}
			if (Run()) TimerCtrl(0b1010); // RUN이 1이면 STOP
			else TimerCtrl(0b0110); // RUN이 0이면 START
		}
	}
}