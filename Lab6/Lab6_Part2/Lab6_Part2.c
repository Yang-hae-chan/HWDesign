#define LEDR_BASE				0xFF200000

static volatile int *LEDR_ptr = (int*) LEDR_BASE;
//LEDR_BASE주소 복사
int LIST[8] = {7, 4, 5, 3, 6, 1, 8, 2};
//TEST_NUMBERS
void display_led(int num){
    *LEDR_ptr=num;//LED출력
}
int main(){
    int max=0;
    int i;
    for(i=1;i<=LIST[0];i++){
        if(max<LIST[i]) max=LIST[i];
    }
    display_led(max);
    //max를 LED로 출력
}