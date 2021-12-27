#include "bv.h"
#include "bf.h"
#include "speck.h"
#include "ht.h"
#include "bst.h"
#include "parser.h"
#include "salts.h"
#include "messages.h"
#include "node.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define OPTIONS "ht:f:s"
#include <unistd.h>
#include <regex.h>
#define WORD "[a-zA-Z0-9_'-]+"
#include <ctype.h>
uint64_t lookups; //get the lookups from ht
uint64_t branches; //get the branches from bst
/////////////////////////////
//Function: int main(int argc, char **argv)
//Description: This function is the culmination of all of the ADTs made previously. By utilizing each of the ADTs, a file is read and the program determines whether or not a thought crime has occured, or whether the person needs counseling on their speech.
//Parameters: int argc, char **argv represent the command line options that are to be passed in
//Returns: An integer determining its success rate
///////////////////////////////
int main(int argc, char **argv) {
    //Initialize the default values for hash size, bloom size, and set stats to false
    int opt = 0;
    bool stats = false;
    uint32_t hash_size = pow(2, 16);
    uint32_t bloom_size = pow(2, 20);
    //while there are still command ops to be read, loop through the line and determine the actions that need to be taken
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h':
            printf("SYNOPSIS\nA word filtering program for the GPRSC.\nFilters out and reports bad "
                   "words parsed from stdin.\nUSAGE\n./banhammer [-hs] [-t size] [-f "
                   "size]\nOPTIONS\n-h           Program usage and help.\n-s           Print "
                   "program statistics.\n-t size      Specify hash table size (default: 2^16).\n-f "
                   "size      Specify Bloom filter size (default: 2^20).");
            break;
        case 't': hash_size = strtoul(optarg, NULL, 10); break;
        case 'f': bloom_size = strtoul(optarg, NULL, 10); break;
        case 's': stats = true; break;
        default: return 0;
        }
    }
    //Initialize the Bloom Filter and the Hash Table
    BloomFilter *bf = bf_create(bloom_size);
    HashTable *ht = ht_create(hash_size);
    FILE *badspeakf = fopen("badspeak.txt", "r");
    char *badspeak = malloc(8 * 64);
    //while there are still words to scan in the badspeak file, add the word scanned in to the bloom filter and the hash table
    while (fscanf(badspeakf, "%s", badspeak) != EOF) {
        bf_insert(bf, badspeak);
        ht_insert(ht, badspeak, NULL);
    }
    //close the files used
    fclose(badspeakf);
    free(badspeak);
    FILE *newspeakf = fopen("newspeak.txt", "r");
    char *newspeak = malloc(8 * 64);
    char *oldspeak = malloc(8 * 64);
    //while there are still words to scan in the newspeak file, add the oldspeak to the bloom filter and the hash table insert the oldspeak and newspeak translation
    while (fscanf(newspeakf, "%s %s", oldspeak, newspeak) != EOF) {
        bf_insert(bf, oldspeak);
        ht_insert(ht, oldspeak, newspeak);
    }
    fclose(newspeakf);
    free(oldspeak);
    free(newspeak);
    //create a regex
    regex_t regular_expression;
    //if it does not meet the criteria defined in the WORD definition, then print the error
    if (regcomp(&regular_expression, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        return 1;
    }

    char *word = NULL;
    Node *badSpeakTree = bst_create(); //create a tree for the badspeak words
    Node *rightSpeakTree = bst_create(); //create a tree for the right speak words
    //booleans determining which message to print
    bool thoughtPolice = false;
    bool bigBrotherCounsel = false;
    //iterate through the file(standard in)
    while ((word = next_word(stdin, &regular_expression)) != NULL) {
        //for case insensitive words, make the letters all lower case
        //////////////////////////////
        //Citations: While looking up how to use the tolower funciton, I utilized code from https://stackoverflow.com/questions/2661766/how-do-i-lowercase-a-string-in-c
        for (int i = 0; word[i]; i++) {
            word[i] = tolower(word[i]);
        }
        //////////////////////////////

        //If the word is in the bloom filter
        if (bf_probe(bf, word) && word != NULL) {
            Node *check_word = (ht_lookup(ht, word));
            //look up the word in the hash table
            //if it does not have a newspeak translation, then the word must be a badspeak word
            if ((check_word != NULL) && (check_word->newspeak == NULL)) {
                thoughtPolice = true;
                lookups += 1;
                badSpeakTree
                    = bst_insert(badSpeakTree, word, NULL); //add to badspeak binary search tree
            }
            //if the word has a newspeak translation, then that means the word goes in the right speak tree
            if ((check_word != NULL) && (check_word)->newspeak != NULL) {
                bigBrotherCounsel = true;
                lookups += 1;
                rightSpeakTree = bst_insert(rightSpeakTree, word,
                    ht_lookup(ht, word)->newspeak); //add the word into the right speak tree
            }
        }
    }
    regfree(&regular_expression); //clear the regex
    clear_words(); //clear the words
    free(word); //free the word used to represent the read in word from the file
    //if the stats option is not selected, then print out the messages
    if (!stats) {
        if (thoughtPolice && bigBrotherCounsel) { //if both booleans, print out the mixspeak message
            printf("%s", mixspeak_message);
            bst_print(badSpeakTree);
            bst_print(rightSpeakTree);
        } else if (thoughtPolice) { //if the thought police boolean, then the badspeak message
            printf("%s", badspeak_message);
            bst_print(badSpeakTree);
        } else if (bigBrotherCounsel) { //if the counsel boolean, then the good speak message
            printf("%s", goodspeak_message);
            bst_print(rightSpeakTree);
        }
    }
    if (stats) { //if the stats option is selected print out the average bst height,size, the average branches traversed, the hash table load, and the bloom filter load
        fprintf(stdout, "Average BST size: %lf\n", ht_avg_bst_size(ht));
        fprintf(stdout, "Average BST height: %lf\n", ht_avg_bst_height(ht));
        fprintf(
            stdout, "Average branches traversed: %lf\n", (double) branches / ((double) lookups));
        fprintf(
            stdout, "Hash table load: %lf", (100.0 * (double) ht_count(ht) / (double) ht_size(ht)));
        fprintf(stdout, "%%\n");
        fprintf(stdout, "Bloom filter load: %lf",
            100.0 * ((double) bf_count(bf) / (double) (bf_size(bf))));
        printf("%%\n");
    }
    //memory clears
    bf_delete(&bf);
    ht_delete(&ht);
    bst_delete(&rightSpeakTree);
    bst_delete(&badSpeakTree);
}
