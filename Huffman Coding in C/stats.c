#include "stats.h"
#include "node.h"
int cmp(uint32_t x, uint32_t y) {
    if (x < y) {
        return -1;
    } else if (x > y) {
        return 1;
    } else {
        return 0;
    }
}
void swap(Node **x, Node **y) {
    Node *t = *x;
    *x = *y;
    *y = t;
}
