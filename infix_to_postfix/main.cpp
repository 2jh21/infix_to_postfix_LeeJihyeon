#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STACK_SIZE 100

typedef char element;

typedef struct {
    element data[MAX_STACK_SIZE];
    int top;
}StackType;

void init_stack(StackType* s) {
    s->top = -1;
}

int is_empty(StackType* s) {
    return(s->top == -1);
}

int is_full(StackType* s) {
    return(s->top == (MAX_STACK_SIZE - 1));
}

void push(StackType* s, element item) {
    if (is_full(s)) {
        fprintf(stderr, "Stack Full!\n");
        return;
    }
    else s->data[++(s->top)] = item;
}

element pop(StackType* s) {
    if (is_empty(s)) {
        fprintf(stderr, "Stack Empty!\n");
        return 0;
    }
    else return s->data[(s->top)--];
}

element peek(StackType* s) {
    if (is_empty(s)) {
        fprintf(stderr, "Stack Empty!\n");
        return 0;
    }
    else return s->data[(s->top)];
}

int prec(char op)
{
    switch (op) {
    case '(': case ')': return 0;
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    }
    return -1;
}

int valid_expression(char exp[]) {
    int i, paren_count = 0;

    for (i = 0; i < strlen(exp); i++) {
        char ch = exp[i];

        if (!(ch >= '0' && ch <= '9') && ch != '(' && ch != ')' && ch != '+' && ch != '-' && ch != '*' && ch != '/') {
            fprintf(stderr, "Error: Invalid character!\n\n");
            return 0;
        }

        if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            if (i == 0 || (i > 0 && (exp[i - 1] == '(' || exp[i - 1] == '+' || exp[i - 1] == '-' || exp[i - 1] == '*' || exp[i - 1] == '/'))) {
                fprintf(stderr, "Error: Invalid expression!\n\n");
                return 0;
            }
        }
        else if (ch == '(') {
            paren_count++;
        }
        else if (ch == ')') {
            paren_count--;
            if (paren_count < 0) {
                fprintf(stderr, "Error: Mismatched parentheses!\n\n");
                return 0;
            }
        }
    }

    if (paren_count != 0) {
        fprintf(stderr, "Error: Mismatched parentheses!\n\n");
        return 0;
    }

    return 1;
}

void infix_to_postfix(char exp[], char postfix[]) {
    int i = 0, j = 0;
    char ch, top_op;
    StackType s;
    init_stack(&s);
    for (i = 0; i < strlen(exp); i++) {
        ch = exp[i];
        switch (ch) {
        case '+': case '-': case '*': case '/':
            while (!is_empty(&s) && (prec(ch) <= prec(peek(&s))))
                postfix[j++] = pop(&s);
            push(&s, ch);
            break;
        case '(':
            push(&s, ch);
            break;
        case ')':
            top_op = pop(&s);
            while (top_op != '(') {
                postfix[j++] = top_op;
                top_op = pop(&s);
            }
            break;
        default:
            postfix[j++] = ch;
            break;
        }
    }
    while (!is_empty(&s))
        postfix[j++] = pop(&s);
    postfix[j] = '\0';
}

int eval(char exp[]) {
    int op1, op2, i;
    char ch;
    StackType s;

    init_stack(&s);
    for (i = 0; i < strlen(exp); i++) {
        ch = exp[i];

        if (ch >= '0' && ch <= '9') {
            push(&s, ch - '0');
        }
        else {
            op2 = pop(&s);
            op1 = pop(&s);
            switch (ch) {
            case'+': push(&s, op1 + op2); break;
            case'-': push(&s, op1 - op2); break;
            case'*': push(&s, op1 * op2); break;
            case'/': push(&s, op1 / op2); break;
            }
        }
    }
    return pop(&s);
}

int main(void) {
    char infix[MAX_STACK_SIZE];
    char postfix[MAX_STACK_SIZE];

    while (1) {
        printf("Enter an infix expression: ");
        scanf_s("%s", infix, MAX_STACK_SIZE);

        if (valid_expression(infix)) {
            break;
        }
    }

    infix_to_postfix(infix, postfix);
    printf("Postfix expression: %s\n", postfix);
    printf("Result: %d\n", eval(postfix));

    return 0;
}