#include <stdio.h>
#define MAX 100
int stack[MAX];
int top;
void init_stack(void) {
    top = -1;
}
int is_operator(int k) {
    return (k == '+' || k == '-' ||
        k == '*' || k == '/');
}
void print_stack(void) {
    int i;
    printf("\n Stack contents : Top --> Bottom\n");
    for (i = top; i >= 0; i--)
        printf("%-6c", stack[i]);
}
int pop(void) {
    if (top < 0) {
        printf("\n Stack underflow.");
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
main() {
    char* x =  "1 + 6 / (4 - 2 * 3) * 4";
    char* y = malloc(30);
    int i = 0;
    postfix(y, x);
    printf("ans:%d",calc(y));

    
}