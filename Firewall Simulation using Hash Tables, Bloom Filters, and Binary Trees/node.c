#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (oldspeak == NULL) {
        free(n);
        return NULL;
    }
    if (n) {
        n->oldspeak = strdup(oldspeak);
        n->newspeak = NULL;
        if (newspeak != NULL) {
            n->newspeak = strdup(newspeak);
        }
        n->left = NULL;
        n->right = NULL;
    }
    return n;
}
void node_delete(Node **n) {
    if (*n) {
        if ((*n)->oldspeak) {
            free((*n)->oldspeak);
            (*n)->oldspeak = NULL;
        }
        if ((*n)->newspeak) {
            free((*n)->newspeak);
            (*n)->newspeak = NULL;
        }
        free((*n));
        *n = NULL; //make if statement for n, one for oldspeak, and one for newspeak inside of the if(*n)
    }
}
void node_print(Node *n) {
    if (n->oldspeak != NULL && n->newspeak != NULL) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    }
    if (n->oldspeak != NULL && n->newspeak == NULL) {
        printf("%s\n", n->oldspeak);
    }
}
