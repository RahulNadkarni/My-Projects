#include "node.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"
#include "ht.h"
uint64_t branches = 0; //Branches external variable declared to zero
Node *bst_create(void) {
    return NULL;
}
/////////////////////////////
//Function: bst_height(Node *root)
//Description: This function recursively finds the height of a binary search tree. It first goes through the left subtree, then the right.
//Parameter: (Node *root) is a pointer pointing to the root of the binary search tree
//Returns: Returns an unsigned thirty two bit integer which represents the maximum value of the left subtree or the right subtree
////////////////////////////
uint32_t bst_height(Node *root) {
    if (root == NULL) {
        return 0;
    } else {
        //Add one to leftMax and rightMax to account for the root
        uint32_t leftMax = 1 + bst_height(root->left);
        uint32_t rightMax = 1 + bst_height(root->right);
        if (leftMax > rightMax) {
            return leftMax;
        } else {
            return rightMax;
        }
    }
}
/////////////////////////////
//Function: bst_size(Node *root)
//Description: Similarly to the height function, this function recursively goes throught the tree and counts each of the nodes
//Parameter: (Node *root) is a pointer pointing to the root of the binary search tree
//Returns: This function returns a unsigned thrity two bit integer representing the number of nodes in a binary search tree
////////////////////////////
uint32_t bst_size(Node *root) {
    uint32_t size = 0;
    if (root == NULL) {
        return size;
    } else {
        size += 1;
        //Instead of adding at each level like bst_height, use plus equals to add to the overall node size
        size += bst_size(root->left);
        size += bst_size(root->right);
    }
    return size;
}
///////////////////////////////
//Function: Node *bst_find(Node *root, char *oldspeak)
//Description: Goes through the binary tree and searches for the oldspeak word passed in
//Parameter One: (Node *root) is a pointer pointing to the root of the binary search tree
//Parameter Two: (char *oldspeak) is the word that needs to be found in the tree
//Returns: If the word is found in the tree, return that node. If its not found, then the root is a null pointer and gets returned regardless
//Citations: This code was heavily inspired by Professor Darrell Long's code from the Trees and Binary Search Trees Lecture. More specifically, Slide 57
///////////////////////////////
Node *bst_find(Node *root, char *oldspeak) {

    if (root) {
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            branches += 1;
            return bst_find(root->left, oldspeak);
        }
        if (strcmp(root->oldspeak, oldspeak) < 0) {
            branches += 1;
            return bst_find(root->right, oldspeak);
        }
    }

    return root;
}
/////////////////////////////
//Function: Node *bst_insert(Node *root, char *oldspeak, char *newspeak)
//Description: Goes through the binary search tree and looks for the apporpriate area to insert the new node
//Parameter One: (Node *root) is a pointer pointing to the root of the binary search tree
//Parameter Two: (char *oldspeak) is the word that needs to be found in the tree
//Parameter Three: (char *newspeak) represents is the translation of the oldspeak word
//Citations: This code was heavily inspired by Professor Darrell Long's code from the Trees and Binary Search Trees Lecture. More specifically, Slide 62
Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    if (oldspeak == NULL) {
        return NULL;
    }
    if (root) {
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            branches += 1;
            root->left = bst_insert(root->left, oldspeak, newspeak);
        }
        if (strcmp(root->oldspeak, oldspeak) < 0) {
            branches += 1;
            root->right = bst_insert(root->right, oldspeak, newspeak);
        }

        return root;
    }

    return node_create(oldspeak, newspeak);
}
//////////////////////////////
//Function: void bst_print(Node *root)
//Description: This function prints out the binary search tree using an in order traversal
//Parameter One: (Node *root) is a pointer pointing to the root of the binary search tree
//Returns: Nothing, this is a void function
/////////////////////////////
void bst_print(Node *root) {
    if (root) {
        bst_print(root->left);
        node_print(root);
        bst_print(root->right);
    }
}
//////////////////////////////
//Function: void bst_delete(Node **root)
//Description: This function uses a post order traversal to delete the nodes in the binary search tree
//Parameter: (Node *root) is a pointer pointing to the root of the binary search tree
//Returns: Nothing, this is a void function
////////////////////////////
void bst_delete(Node **root) {
    if (*root) {
        bst_delete(&(*root)->left);
        bst_delete(&(*root)->right);
        node_delete(root);
    }
}
