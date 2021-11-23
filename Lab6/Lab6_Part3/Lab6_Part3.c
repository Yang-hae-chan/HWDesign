#define HEX3_HEX0_BASE			0xFF200020
#define HEX5_HEX4_BASE			0xFF200030
#include <stdio.h>

unsigned int TEST_NUM[] = { 0x0000e000, 0x3fabedef, 0x00000001, 0x00000002,
                           0x75a5a5a5, 0x01ffc000, 0x03ffc000, 0x55555555,
                           0x77777777, 0x08888888,0x00000000 };
unsigned int seg_code[] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111,
                            0b01100110, 0b01101101, 0b01111101, 0b00000111,
                            0b01111111, 0b01100111 };
                            //segment code
static volatile int* HEX3_HEX0_ptr = (int*)HEX3_HEX0_BASE;
static volatile int* HEX5_HEX4_ptr = (int*)HEX5_HEX4_BASE;
//HEX에 diplay하기위한 함수
void display_HEX(int num, int hex) {
    int hex_tmp = hex % 4;//hex3-0과 5-4는 주소값이 다르므로
    int NT,NO;//NT=10의자리수 NO=1의자리수
    unsigned int output_num;//hex에 저장할 segmentcode
    if (num >= 10) {     //10이상일때
        NT = num / 10;  //입력값을 10으로 나눠 10의자리수를 구함
        NO = num - 10*NT;   //1의자리수 구함
    }
    else {          //10미만
        NO = num;   //1의자리 그대로출력
        NT = 0;     //10의자리는 0
    }
    unsigned int output_num_T = seg_code[NT] << (hex_tmp * 8);
    //10의자리 segment code부분
    unsigned int output_num_O = seg_code[NO] << ((hex_tmp-1) * 8);
    //1의자리 segment code부분
    output_num=output_num_O|output_num_T|* HEX3_HEX0_ptr;
    //10자리 1의자리 합침
    if (hex > 3) {//HEX4나 HEX5로 출력할 때
        output_num=output_num_O|output_num_T;
        *HEX5_HEX4_ptr = output_num;
    }
    else {//HEX3-0
        output_num=output_num_O|output_num_T|* HEX3_HEX0_ptr;
        *HEX3_HEX0_ptr = output_num;
    }
}
int COUNT_ONES() {//ones
    int i, j, ones = 0, ones_tmp = 0, oness = 0;
    //ones:testnum하나에서의 가장 큰 ones
    //ones_tmp:testnum하나에서의 현재ones값
    //oness:최종 ones값
    unsigned int l;//testnum임시저장용
    unsigned int k = 0x80000000;
    for (i = 0;i < sizeof(TEST_NUM) / sizeof(int);i++) {
        l = TEST_NUM[i];
		if(l==0x0) break;//0x0이면 break
        ones = 0;
        for (j = 0;j < 32;j++) {
            if ((l & k) == 0) {
                if (ones_tmp > ones) {
                    ones = ones_tmp;
                }
                ones_tmp = 0;
            }
            else {
                ones_tmp++;
            }
            l = l << 1;
        }
        if (ones > oness) oness = ones;
    }
    return oness;
}
int COUNT_ZEROS() {
    int i, j, ones = 0, ones_tmp = 0, oness = 0;
    //ones:testnum하나에서의 가장 큰 ones
    //ones_tmp:testnum하나에서의 현재ones값
    //oness:최종 ones값
    unsigned int l;
    unsigned int k = 0x80000000;
    for (i = 0;i < sizeof(TEST_NUM) / sizeof(int);i++) {
        if(TEST_NUM[i]==0x0) break;//0x0이면 break
		l = ~TEST_NUM[i];//zeros는 ones에서 testnum만 inverse해서 구하면된다
        ones = 0;
        for (j = 0;j < 32;j++) {
            if ((l & k) == 0) {
                if (ones_tmp > ones) {
                    ones = ones_tmp;
                }
                ones_tmp = 0;
            }
            else {
                ones_tmp++;
            }
            l = l << 1;
        }
        if (ones > oness) oness = ones;
    }
    return oness;
}

int COUNT_ALTERNATE() {
    int i, j, ones = 0, ones_tmp = 0, oness = 0;
    unsigned int l, m;//토글되는 비트를 1로 만들어주기위함
    unsigned int k = 0x80000000;
    //ones:testnum하나에서의 가장 큰 ones
    //ones_tmp:testnum하나에서의 현재ones값
    //oness:최종 ones값
    for (i = 0;i < sizeof(TEST_NUM) / sizeof(int);i++) {
		if(TEST_NUM[i]==0x0) break;//0x0이면 break
        if ((TEST_NUM[i] << 31) == 0) l = TEST_NUM[i];
        else    l = ~TEST_NUM[i];
        m = l << 1;
        l = l ^ m;
        ones = 0;
        for (j = 0;j < 32;j++) {
            if ((l & k) == 0) {
                if (ones_tmp > ones) {
                    ones = ones_tmp;
                }
                ones_tmp = 0;
            }
            else {
                ones_tmp++;
            }
            l = l << 1;
        }
        if (ones > oness) oness = ones;
    }
    if (oness > 0) oness++;
    return oness;
}

int main() {
	* HEX3_HEX0_ptr=0;
	* HEX5_HEX4_ptr =0;
    int ones=COUNT_ONES();
    int zeros=COUNT_ZEROS();
    int alter=COUNT_ALTERNATE();
    display_HEX(ones, 1);//ones를 HEX1,0에 출력
    display_HEX(zeros, 3);//zeros를 HEX3,2에 출력
    display_HEX(alter, 5);//alternate를 HEX5,4에 출력
    printf("ONES: %d \n",ones);
    printf("ZEROS: %d \n",zeros);
    printf("ALTERNATES: %d \n",alter);
}