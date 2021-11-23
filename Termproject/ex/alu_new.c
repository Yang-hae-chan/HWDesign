#include <stdio.h>
#include <stdlib.h>
typedef struct _node {
    int key;
    struct _node* next;
} node;
node* head, * tail;
void init_stack(void) {
    head = (node*)malloc(sizeof(node));
    tail = (node*)malloc(sizeof(node));
    head->next = tail;
    tail->next = tail;
}
int push(int k) {
    node* t;
    if ((t = (node*)malloc(sizeof(node))) == NULL) {
        printf("\n Out of memory...");
        return -1;
    }
    t->key = k; t->next = head->next; head->next = t;
    return k;
}
int pop(void) {
    node* t;
    int i;
    if (head->next == tail) /* if empty */ {
        printf("\n Stack underflow.");
        return -1;
    }
    t = head->next; i = t->key;
    head->next = t->next; free(t);
    return i;
}

int is_operator(char k) {
    return (k == '+' || k == '-' ||
        k == '*' || k == '/');
}

int precedence(int op) {
    if (op == '(') return 0;
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    else return 3;
}

int calc(char* p) {
    int i;
    init_stack();
    while (*p) {
        while (*p == ' ') p++;
        if (*p == '\0') break;
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
    char* x = "1 6 4 2 3 * - / 4 * +";
    int i = 0;
    //postfix(y, x);
    printf("ans:%d",calc(x));
}