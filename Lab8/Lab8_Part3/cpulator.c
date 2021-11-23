#define PIXEL_BUF_CTRL_BASE		0xFF203020
#include <stdio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define PIXEL(r,g,b) (short int)((((r)&0x1f)<<11)|(((g)&0x3f)<<5)|(((b)&0x1f)))
#define FILL_PIXEL(x,y,r,g,b) *(short int*)(pixel_buffer_start +(((y)&0xff)<<10)+ (((x)&0x1ff)<<1))=PIXEL(r,g,b)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
volatile int pixel_buffer_start;
volatile int* pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;
short int front_buffer[512*256];
short int back_buffer[512*256];
void clear_screen(int r, int g, int b){
    int x, y;
    for(x=0;x<SCREEN_WIDTH+1;x++){
        for(y=0;y<SCREEN_HEIGHT+1;y++){
            FILL_PIXEL(x,y,r,g,b);
        }
    }
}
int abs(int x){
    if(x<0)
    return -x;
    else
    return x;
}
void draw_line(int x1, int y1, int x2, int y2, int r, int g, int b){
    int tmp,x,y;
    if(abs(x2-x1)<abs(y2-y1)){
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
        if(abs(x2-x1)<abs(y2-y1)){
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
void draw_square(int x1, int y1, int x2, int y2, int r, int g, int b) {
   int x, y;
   for (x = x1; x <= x2; x++) {
      for (y = y1; y <= y2; y++) {
         FILL_PIXEL(x, y, r, g, b);
      }
   }
}
void wait_for_vsync() {
    register int status;
    *pixel_ctrl_ptr = 1;

    status = *(pixel_ctrl_ptr + 3);
    while ((status & 0x01) != 0)
    status = *(pixel_ctrl_ptr + 3);
}

int main()
{

    pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;
    *(pixel_ctrl_ptr + 1) = front_buffer;
    wait_for_vsync();

    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(0x00, 0x00, 0x00);
    *(pixel_ctrl_ptr + 1) = back_buffer;
    int x0 = 50;
    int x1 = SCREEN_WIDTH - 50;
    int y = 0;
    point* pointer = (point*)malloc(sizeof(point));
    int k = 0;
    while (k < 10) {



        for (int i = 0; i < 8; i++)

        {

            pointer->x[i] = rand() % 20;//100~200사이 난수
            pointer->y[i] = rand() % 40;

        }
        k++;
        printf("\n");
    }
    for (int i = 0; i < 8; i++)

    {
        pointer->x[i] = rand() % SCREEN_WIDTH;//100~200사이 난수
        pointer->y[i] = rand() % SCREEN_HEIGHT;
    }
    int x_arr[8] = {1,1,1,1,1,1,1,1};
    int y_arr[8] = { 1,1,1,1,1,1,1,1 };
    while (1) {

        pixel_buffer_start = *(pixel_ctrl_ptr + 1);
        clear_screen(0x00, 0x00, 0x00);
        for (int i = 0; i < 7; i++) {
            draw_line(pointer->x[i], pointer->y[i], pointer->x[i + 1], pointer->y[i + 1], 0xffff);
        }
        for (int i = 0; i < 8; i++) {
            pointer->x[i] += x_arr[i];
            pointer->y[i] += y_arr[i];
            if (pointer->x[i] == SCREEN_WIDTH) {
                x_arr[i] = -1;
            }
            else if (pointer->x[i] == 0) {
                x_arr[i] = 1;
            }
            if (pointer->y[i] == SCREEN_HEIGHT) {
                y_arr[i] = -1;
            }
            else if (pointer->y[i] == 0) {
                y_arr[i] = 1;
            }
        }
        wait_for_vsync();
    }
}

