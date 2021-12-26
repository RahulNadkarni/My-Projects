#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "node.h"
#include "pq.h"
#include <math.h>
#include "stats.h"
struct PriorityQueue { //priority queue struct
    uint32_t tail;
    uint32_t capacity;
    uint32_t head;
    Node **arr; //array of nodes
};
void fix_heap(PriorityQueue *q, uint32_t first, uint32_t last);
void heap_sort(PriorityQueue *A, uint32_t n);
void decrement(PriorityQueue *q);
void insertion_sort(PriorityQueue *q, uint32_t n);
PriorityQueue *pq_create(uint32_t capacity) { //a function that creates a priority queue
    PriorityQueue *q
        = (PriorityQueue *) malloc(sizeof(PriorityQueue)); //allocate memory for the queue
    if (q) { //if exists
        q->head = 0; //set to zero
        q->tail = 0; //set tail to zero
        q->capacity = capacity; //set capacity to capacity
        q->arr = (Node **) calloc(capacity, sizeof(Node)); //set the memory for the array
    }
    return q;
}

void pq_delete(PriorityQueue **q) { //a function to delete pq
    if (*q && (*q)->arr) {
        free((*q)->arr); //free the qrray
        free(*q); //free the queue
        *q = NULL;
    }
    return;
}

bool pq_empty(PriorityQueue *q) { //a function that returns true when the queue is empty
    return (q->tail == 0);
}

bool pq_full(PriorityQueue *q) { //a function that returns true when the queue is full
    return (q->tail == q->capacity);
}
uint32_t pq_size(PriorityQueue *q) { //a function that returns the size of the queue
    return q->tail;
}
bool enqueue(PriorityQueue *q, Node *n) { //a function that enqueues a node
    if (pq_full(q)) { //if full return false
        return false;
    }
    q->arr[q->tail] = n; //add node to the pq
    q->tail++; //increase the tail by one
    fix_heap(q, 1, q->tail); //fix the heap
    return true;
}
bool dequeue(
    PriorityQueue *q, Node **n) { //a function that dequeues a node if it has the lowest frequency
    if (pq_empty(q)) { //if empty return false
        return false;
    }
    *n = q->arr[q->head]; //remove the root
    q->arr[q->head] = q->arr[q->tail - 1]; //shift the queue
    q->tail--; //decrease the tail by one
    fix_heap(q, 1, pq_size(q)); //fix the heap again

    return true;
}
void pq_print(PriorityQueue *q) { //a function that prints the queue
    for (uint32_t i = 0; i < q->tail; i = i + 1) {
        node_print(q->arr[i]);
    }
}
uint32_t min_child(PriorityQueue *q, uint32_t first, uint32_t last) { //returns the max child value
    uint32_t left = 2 * first; //create a left variable equal to two times the first
    uint32_t right = left + 1; //the right is equal to the left plus one
    if ((right <= last)
        && (q->arr[right - 1]->frequency
            < q->arr[left - 1]->frequency)) { //check if the right condition is met
        return right; //return the right value if it the condition is met
    }
    return left; //return left otherwise
}
void fix_heap(PriorityQueue *q, uint32_t first, uint32_t last) { //a function that fixes the heap
    bool found = false; //found value is false
    uint32_t mother = first; //mother set to first
    uint32_t small = min_child(q, mother, last); //small set to the min value
    while (mother <= floor(last / 2) && (found == false)) //while condition is met
    {
        if (q->arr[mother - 1]->frequency
            > q->arr[small - 1]
                  ->frequency) { //if the node frequency is less than the other frequency
            Node *swp = q->arr[mother - 1]; //create a pointer to the first node
            q->arr[mother - 1] = q->arr[small - 1]; //swap the nodes
            q->arr[small - 1] = swp; //set the pointer to the now swapped nodes
            mother = small; //set mother to small
            small = min_child(q, mother, last); //get the new min value
        } else {
            found = true; //true
        }
    }
}
