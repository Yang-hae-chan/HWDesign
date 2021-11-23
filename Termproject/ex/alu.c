#include <stdio.h>
#include "address_map_nios2.h"
#define MAX 100
extern int stack[MAX];
extern int top;
int errorr=0;
void error();
void init_stack(void) {
    top = -1;
}
int is_operator(char k) {
    return (k == '+' || k == '-' ||
        k == '*' || k == '/');
}
void print_stack(void) {
    int i;
    printf("\n Stack contents : Top --> Bottom\n");
    for (i = top; i >= 0; i--)
        printf("%c", stack[i]);
}
int pop(void) {
    if (top < 0) {
        printf("\n Stack underflow.");
        error();
        errorr=1;
        top=-1;
        clear_input();
        return -1;
    }
    return stack[top--];
}
int push(int t) {
    if (top >= MAX - 1) {
        printf("\n Stack overflow.");
        return -1;
    }
    stack[++top] = t;
    return t;
}

int precedence(int op) {
    if (op == '(') return 0;
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    else return 3;
}
int get_stack_top(void) {
    return (top < 0) ? -1 : stack[top];
}
int is_stack_empty(void) {
    return (top < 0);
}

void postfix(char* dst, char* src) {
    char c;
    init_stack();
    while (*src) {
        if(errorr) {errorr=0; break;}
        while (*src == ' ') src++;
        if (*src== '\0') break;
        if (*src == '(') { push(*src); src++; }
        else if (*src == ')') {
            while (get_stack_top() != '(') {
                *dst++ = pop(); *dst++ = ' ';
                printf("post: %c", *src);
            }
            pop(); src++;
            print_stack(); printf("\n");
        }
        else if (is_operator(*src)) {
            while (!is_stack_empty() &&
                precedence(get_stack_top()) >=
                precedence(*src)) {
                *dst++ = pop(); *dst++ = ' ';
                printf("post: %c", *src);
            }
            push(*src); src++;
            print_stack(); printf("\n");
        }
        else if (*src >= '0' && *src <= '9') {
            do {printf("post: %c", *src);
                *dst++ = *src++;
            } while (*src >= '0' && *src <= '9');
            
            *dst++ = ' ';
        }
        else src++;
    }
    while (!is_stack_empty()) {
        *dst++ = pop(); *dst++ = ' ';
    }
    dst--; *dst = 0;
}
int calc(char* p) {
    int i;
    init_stack();
    while (*p) {
        while (*p == ' ') p++;
        if (*p== '\0') break;
        if (*p >= '0' && *p <= '9') {
            i = 0;
            do {
                i = i * 10 + *p - '0';
                p++;
            } while (*p >= '0' && *p <= '9');
            push(i);
        }
        else if (*p == '+') {
            push(pop() + pop());
            p++;
        }
        else if (*p == '*') {
            push(pop() * pop());
            p++;
        }
        else if (*p == '-') {
            i = pop();
            push(pop() - i);
            p++;
        }
        else if (*p == '/') {
            i = pop();
            push(pop() / i);
            p++;
        }
        else p++;
    }
    return pop();
}
void error(void){
    volatile int * audio_ptr   = (int *)AUDIO_BASE;
    int i=0;
    int buffer[11]={0, 2147483640,-2147483640,2147483640,-2147483640,2147483640,-2147483640,2147483640,-2147483640,2147483640,-2147483640};
    for(int j=0; j<1000000;j++) {
        *(audio_ptr + 2) = buffer[i];
        *(audio_ptr + 3) = buffer[i];
        i++;
        i%=10;
    }
}
/*main() {
    char* x =  "1 + 6 / (4 - 2 * 3) * 4";
    char* y = malloc(30);
    int i = 0;
    postfix(y, x);
    printf("ans:%d",calc(y));    
}*/