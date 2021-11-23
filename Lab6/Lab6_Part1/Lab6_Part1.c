#include <stdio.h>

int LIST[8] = {7, 4, 5, 3, 6, 1, 8, 2};
//TEST NUNBERS
int main(){
    int max=0;//max초기화
    int i;
    for(i=1;i<=LIST[0];i++){
        if(max<LIST[i]) max=LIST[i];
    }
    printf("max=%d",max);//max출력
}