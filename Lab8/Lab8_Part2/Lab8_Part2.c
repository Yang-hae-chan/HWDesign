#define PIXEL_BUF_CTRL_BASE		0xFF203020

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
//표시할 스크린의 크기 상수 정의 
#define PIXEL(r,g,b) (short int)((((r)&0x1f)<<11)|(((g)&0x3f)<<5)|(((b)&0x1f)))
#define FILL_PIXEL(x,y,r,g,b) *(short int*)(pixel_buffer_start +(((y)&0xff)<<10)+ (((x)&0x1ff)<<1))=PIXEL(r,g,b)

volatile int pixel_buffer_start;//화면 저장
volatile int* pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;
//전역 번수 전언
short int front_buffer[512*256]; //front buffer를 위한 메모리 할당
short int back_buffer[512*256]; //back buffer를 위한 메모리 할당
void clear_screen(int r, int g, int b){//인수로 받은 rgb값으로 clear
    int x, y;
    for(x=0;x<SCREEN_WIDTH+1;x++){
        for(y=0;y<SCREEN_HEIGHT+1;y++){
            FILL_PIXEL(x,y,r,g,b);
        }
    }
}
int abs(int x){//절댓값
    if(x<0)
    return -x;
    else
    return x;
}
void draw_line(int x1, int y1, int x2, int y2, int r, int g, int b){
    //선을 그리는 함수, 강의자료에 있는 알고리즘 사용함 설명 생략
    int tmp,x,y;
    int grad=(abs(x2-x1)<abs(y2-y1));
    if(grad){
        tmp=x2;
        x2=y2;
        y2=tmp;
        tmp=x1;
        x1=y1;
        y1=tmp;
    }
    if(x1>x2){
        tmp=x1;
        x1=x2;
        x2=tmp;
        tmp=y1;
        y1=y2;
        y2=tmp;
    }
    int dx=abs(x2-x1);
    int dy=abs(y2-y1);
    int err=-(dx/2);
    y=y1;
    int y_step;
    if(y1<y2) y_step=1;
    else y_step=-1;
    for(x=x1;x<=x2;x++){
        if(grad){
            FILL_PIXEL(y,x,r,g,b);
        }
        else FILL_PIXEL(x,y,r,g,b);
        err=err+dy;
        if(err>=0){
            y+=y_step;
            err-=dx;
        }
    }

}

void wait_for_vsync() {
    register int status;
    *pixel_ctrl_ptr = 1;
    //synchronization process 시작(swappint)
    status = *(pixel_ctrl_ptr + 3);//S
    while ((status & 0x01) != 0)//S=0(swapping이 끝날때)까지 대기
    status = *(pixel_ctrl_ptr + 3);
}

int main(void) {
    *(pixel_ctrl_ptr+1)=front_buffer; //front buffer로 주소 할당
    wait_for_vsync();                   //vsync 대기
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(0x00, 0x00, 0x00); //화면 검은색으로 clear
    *(pixel_ctrl_ptr+1)=back_buffer;//back buffer로 주소 할당
    int x = 60, y = 0, a=-1;//초기 좌표
    while (1) {
        pixel_buffer_start = *(pixel_ctrl_ptr+1);
        clear_screen(0x00, 0x00, 0x00); //검은색으로 clear
        if(y>=SCREEN_HEIGHT||y<=0) a++; //screen 끝까지 가면 a++
        if(a%2!=0) y-=1;//a가 홀수이면 선이 위로 올라오게
        else y+=1;//짝수이면 아래로
        draw_line(x, y, x + 200, y, 0xff, 0xff, 0xff);//선 그림
        wait_for_vsync(); //vsync 대기
    }
}

