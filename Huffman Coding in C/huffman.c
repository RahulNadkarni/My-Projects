#include "huffman.h"

#include "node.h"
#include "pq.h"
#include "stack.h"
#include "io.h"
#include <unistd.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

uint32_t symbols = 0;

void recurse(Node *root, Code table[static ALPHABET], Code c);

Node *build_tree(uint64_t hist[static ALPHABET]) { // a function that builds the tree
    PriorityQueue *q = pq_create(ALPHABET); //create a new pq
    for (uint64_t i = 0; i < ALPHABET; i++) //increment through alphabet
    {
        if (hist[i] != 0) { //if not zero
            Node *temp = node_create(i, hist[i]); //create the node
            enqueue(q, temp); //enqueue
            symbols += 1;
        }
    }
    Node *left, *right;
    while (pq_size(q) != 1) {
        dequeue(q, &left);
        dequeue(q, &right);
        Node *n = node_join(left, right);
        enqueue(q, n);
    }
    Node *p;
    dequeue(q, &p);
    pq_delete(&q);
    return p;
}
void build_codes(Node *root, Code table[static ALPHABET]) {
    Code c = code_init();
    recurse(root, table, c);
    return;
}
void recurse(Node *root, Code table[static ALPHABET], Code c) {
    if (root) {
        uint8_t bit;
        if (root->left == NULL && root->right == NULL) {
            table[root->symbol] = c;
        } else {
            code_push_bit(&c, 0);
            recurse(root->left, table, c);
            code_pop_bit(&c, &bit);
            code_push_bit(&c, 1);
            recurse(root->right, table, c);
            code_pop_bit(&c, &bit);
        }
    }
    return;
}
Node *rebuild_tree(
    uint16_t nbytes, uint8_t tree_dump[static nbytes]) //a function that rebuilds tree
{
    Node *root;
    Stack *s = stack_create(nbytes);
    for (uint16_t i = 0; i < nbytes; i++) {
        if (tree_dump[i] == 'L') {
            Node *n = node_create(tree_dump[i + 1], 0);
            stack_push(s, n);
        } else if (tree_dump[i] == 'I') {
            Node *right;
            Node *left;
            stack_pop(s, &right);
            stack_pop(s, &left);
            Node *parent = node_join(left, right);
            stack_push(s, parent);
        }
        if (i != nbytes - 1) {
            if ((tree_dump[i + 1] == 'I' || tree_dump[i + 1] == 'L') && tree_dump[i] == 'L') {
                tree_dump[i + 1] = '\0';
            }
        }
    }
    stack_pop(s, &root);
    stack_delete(&s);
    return root;
}
void delete_tree(Node **root) {
    if (*root) {
        if ((*root)->left) { // Traverse and delete left Nodes
            delete_tree(&(*root)->left);
        }
        if ((*root)->right) { // Traverse and delete right Nodes
            delete_tree(&(*root)->right);
        }
        node_delete(root); // Delete Node
    }
}
void dump_tree(int outfile, Node *root) { //a function that writes the code
    if (root) { //if exists
        dump_tree(outfile, root->left); //post order root left
        dump_tree(outfile, root->right); //post order root right
        if (!(root->left) && !(root->right)) { //if not either roots
            write(outfile, "L", 1); //write L
            write_bytes(outfile, &root->symbol, 1); //write the root symbol
        } else {
            write(outfile, "I", 1); //write Interior code
        }
    }
}
