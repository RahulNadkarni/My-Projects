#include "bst.h"
#include "salts.h"
#include "speck.h"
#include "ht.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

uint64_t lookups = 0; //initialize external variable that represents the number of loookups
//////////////////////////
//Function: HashTable *ht_create(uint32_t size)
//Description: This function initializes a hash table by initializing the salts, then the array of trees
//Parameters: (uint32_t size) is an unsigned thirty two bit integer which represents the size or the number of binary search trees in the trees array
//Returns: This function returns the fully created hash table
/////////////////////////
HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        ht->salt[0] = SALT_HASHTABLE_LO;
        ht->salt[1] = SALT_HASHTABLE_HI;
        ht->trees = (Node **) calloc(size, sizeof(Node));
        if (!ht->trees) {
            free(ht);
            ht = NULL;
        }
        ht->size = size;
    }
    return ht;
}
////////////////////////////
//Funciton:void ht_delete(HashTable **ht)
//Description: This function deletes the hash table / frees the memory that was used while it was being utilized
//Parameter: (HashTable **ht) is a a double pointer to the hash table
//Returns: Nothing, this is a void function
///////////////////////////
void ht_delete(HashTable **ht) {
    if (*ht) {
        //Iterate through the trees array and delete the binary tree if it exists
        for (uint32_t i = 0; i < ht_size(*ht); i++) {
            if ((*ht)->trees[i]) {
                bst_delete(&(*ht)->trees[i]);
            }
        }
        free((*ht)->trees);
        ((*ht)->trees) = NULL;
        free(*ht);
        *ht = NULL;
    }
}
///////////////////////////
//Fucntion: ht_size(HashTable *ht)
//Description: This function gets the size of the hash table as specified in the create function
//Parameter:(HashTable *ht) is a pointer to the hash table
//Returns: an unsigned thrity two bit integer representing the size of the hash table
//////////////////////////
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}
///////////////////////////
//Function: Node *ht_lookup(HashTable *ht, char *oldspeak)
//Description: This function looks up the character in the binary search tree
//Parameter One: (HashTable *ht) is a pointer to the hash table
//Parameter Two: char *oldspeak is the word to look up in the hash table
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    lookups += 1;
    return (bst_find(ht->trees[hash(ht->salt, oldspeak) % ht_size(ht)], oldspeak));
}
/////////////////////////////////
//Function: ht_insert(HashTable *ht, char *oldspeak, char *newspeak)
//Description: This function inserts a node into the binary search tree using bst_insert
//Parameter One: (HashTable *ht) is a pointer to the hash table
//Parameter Two:  char *oldspeak is the oldspeak half of the node to insert in the hash table
//Parameter Three: char *newspeak is the newspeak translation of the oldspeak word in the node
//Returns: Nothing, this is a void function
/////////////////////////////////
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    lookups += 1;
    ht->trees[hash(ht->salt, oldspeak) % ht_size(ht)]
        = bst_insert(ht->trees[hash(ht->salt, oldspeak) % ht_size(ht)], oldspeak, newspeak);
}
//////////////////////////////
//Function: uint32_t ht_count(HashTable *ht)
//Description: This function counts the number of trees there are in the hash table
//Parameter: (HashTable *ht) is a pointer to the hash table
//Returns: An unsigned thirty two bit integer which represents the number of trees in the array
/////////////////////////////
uint32_t ht_count(HashTable *ht) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < ht_size(ht); i++) {
        if (ht->trees[i] != NULL) {
            count += 1;
        }
    }
    return count;
}
/////////////////////////////
//Function: double ht_avg_bst_size(HashTable *ht)
//Description: This function sums the sizes of the trees in the array, then divides it by the total to get the average
//Parameter: (HashTable *ht) is a pointer to the hash table
//Returns: Returns a double representing the average size
////////////////////////////
double ht_avg_bst_size(HashTable *ht) {

    double sumsize = 0;

    for (uint32_t i = 0; i < ht_size(ht); i++) {
        sumsize += bst_size(ht->trees[i]);
    }
    return sumsize / ht_count(ht);
}
/////////////////////////////
//Function: double ht_avg_bst_size(HashTable *ht)
//Description: This function sums the heights of the trees in the array, then divides it by the total to get the average
//Parameter: (HashTable *ht) is a pointer to the hash table
//Returns: Returns a double representing the average height
////////////////////////////
double ht_avg_bst_height(HashTable *ht) {
    double sumheight = 0;
    for (uint32_t i = 0; i < ht_size(ht); i++) {
        sumheight += bst_height(ht->trees[i]);
    }
    return sumheight / ht_count(ht);
}
////////////////////////////
//Function: void ht_print(HashTable *ht)
//Description: This function prints out the hash table
//Parameter: (HashTable *ht) is a pointer to the hash table
//Returns: Returns nothing because this is a void function
///////////////////////////////
void ht_print(HashTable *ht) {
    for (uint32_t i = 0; i < ht_size(ht); i++) {
        bst_print(ht->trees[i]);
    }
}
