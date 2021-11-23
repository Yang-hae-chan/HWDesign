#define PIXEL_BUF_CTRL_BASE		0xFF203020
volatile int pixel_buffer_start;//화면 저장
volatile int* pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
//표시할 스크린의 크기 상수 정의 
#define PIXEL(r,g,b) (short int)((((r)&0x1f)<<11)|(((g)&0x3f)<<5)|(((b)&0x1f)))
#define FILL_PIXEL(x,y,r,g,b) *(short int*)(pixel_buffer_start +(((y)&0xff)<<10)+ (((x)&0x1ff)<<1))=PIXEL(r,g,b)


//전역 번수 전언
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
int main(void) {
    pixel_buffer_start = *pixel_ctrl_ptr;//화면에 그림
    clear_screen(0xFF, 0xFF, 0xFF);//흰색으로 화면clear
    draw_line(0,0,150,150,0x00,0x00,0xFF);//파란선
    draw_line(150,150,319,0,0x00,0xFF,0x00);//초록선
    draw_line(0,239,319,239,0xFF,0x00,0x00);//빨간선
    draw_line(319,0,0,239,0xFF,0x00,0xFF);//파란선
}

