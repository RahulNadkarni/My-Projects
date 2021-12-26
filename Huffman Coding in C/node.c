#include "node.h" //include the node header
#include <inttypes.h> //include the integer types
#include <stdio.h> //include standard io just in case
#include <stdlib.h> //include the standard library

Node *node_create(uint8_t symbol,
    uint64_t frequency) { //a function that will create a node and set its symbol and its frequency
    Node *n = (Node *) malloc(sizeof(Node)); //allocate the memory for the node
    if (n) { //if the node was created
        n->left = NULL; //set the left child to null
        n->right = NULL; //set the right child to null
        n->symbol = symbol; //set the symbol to the symbol passed in
        n->frequency = frequency; //set the frequency to the frequency passed in
    }
    return n; //return the node
}
void node_delete(Node **n) { //a function that will delete a node
    if (*n) { //if the node exists
        free((*n)); //free the node
        *n = NULL; //set the pointer to null
    }
    return;
}

Node *node_join(Node *left,
    Node *right) { //a function that will create a parent node given a left node and a right node
    Node *parent = node_create('$',
        left->frequency
            + right
                  ->frequency); //create a new node withthe $ symbol and a frequency of the sum of its children
    parent->left = left; //set the parent's left child to left
    parent->right = right; ///set the parent's right child to right
    return parent; //return the parent node
}

void node_print(Node *n) { //a debugging function to print the node

    printf("%c", n->symbol); //print the symbol
    printf("%lu", n->frequency); //print the frequency
}
