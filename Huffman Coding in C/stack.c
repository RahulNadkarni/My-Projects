#include "node.h"
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include "stack.h"
#include <stdlib.h>
Stack *stack_create(uint32_t capacity) { //A function that creates a stack
    Stack *s = (Stack *) malloc(sizeof(Stack)); //Allocate memory for the stack
    if (s) { //if not null
        s->top = 0; //set the top of the stack to zero
        s->capacity = capacity; //set the capacity to the input
        s->items = (Node **) calloc(capacity, sizeof(Node)); //allocate the memory for the array
        if (!s->items) { //if does not exist
            free(s); //free s
            s = NULL; //free the pointer
        }
    }
    return s; //return the sstack
}

void stack_delete(Stack **s) { //a function that deletes the stack
    if (*s && (*s)->items) {
        free((*s)->items); //delete the array
        free(*s); //delete the stack pointer
        *s = NULL; //set to null
    }
    return;
}

bool stack_empty(Stack *s) { //a function that checkss if the stack is empty
    return (s->top == 0); //if zero, return true
}

bool stack_full(Stack *s) { //if the sstack is full, return true, else false
    return (s->top == s->capacity);
}

uint32_t stack_size(Stack *s) { //return the size of the stack
    return s->top;
}

bool stack_push(Stack *s, Node *n) { //a function that pushes a node to the stack
    if (stack_full(s)) { //if full
        return false; //return false
    }
    s->items[s->top] = n; //add the node to the top
    s->top++; //increase top by one
    return true; //return true
}

bool stack_pop(Stack *s, Node **n) { //a function that pops the node off the stack
    if (stack_empty(s)) { //if stack empty, return false
        return false;
    }

    *n = s->items[s->top - 1]; //pop a node off
    s->top--; //top goes down by one
    return true;
}

void stack_print(Stack *s) { //a function that prints the stack
    for (uint32_t i = 0; i < s->top; i = i + 1) {
        node_print(s->items[i]);
    }
}
