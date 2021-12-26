#include "code.h"
#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "io.h"
#include "pq.h"
#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define OPTIONS "hvi:o:"
int main(int argc, char **argv) { //main
    bool stats = false; //set to false
    int infile = STDIN_FILENO; //set to standard in
    int outfile = STDOUT_FILENO; //set to standard out
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS))
           != -1) { //read through the command line and do the required functions
        switch (opt) {
        case 'h':
            printf(
                "SYNOPSIS\nA Huffman decoder.\nDecompresses a file using the Huffman coding "
                "algorithm.\nUSAGE\n./decode [-h] [-i infile] [-o outfile]\nOPTIONS\n-h            "
                " Program usage and help.\n-v             Print compression statistics.\n-i infile "
                "     Input file to decompress.\n-o outfile     Output of decompressed data.");
            break;
        case 'i': infile = open(optarg, O_RDONLY); break;
        case 'o': outfile = open(optarg, O_WRONLY | O_APPEND | O_CREAT); break;
        case 'v': stats = true; break;
        }
    }
    Header h;
    read_bytes(infile, (uint8_t *) &h, sizeof(Header)); //read the header
    if (h.magic != MAGIC) { //check if it has the magic number correct
        fprintf(stderr, "Error: invalid Magic number.\n");
        return 1;
    }
    uint8_t tree[h.tree_size];
    fchmod(outfile, h.permissions);
    read_bytes(infile, tree, h.tree_size);
    Node *root = rebuild_tree(h.tree_size, tree); //rebuild tree given encoded information
    Node *n = root;
    uint64_t i = 0;
    uint8_t buf[BLOCK];
    //Decoding Algorithm
    while (i < h.file_size) {
        uint8_t bit;
        if (n->left == NULL && n->right == NULL) {
            buf[i % BLOCK] = n->symbol; //Get the symbol
            n = root; //set the root
            i++; //increment the buffer
            if (i % BLOCK == 0
                && i != 0) { //write to the outfile if i mod block = 0 and i not equal to 0
                write_bytes(outfile, buf, sizeof(buf)); //write to the outfile
            }
        }
        read_bit(infile, &bit); //read the bit
        if (bit == 0 && n->left != NULL) { //if 0 go left
            n = n->left;
        }
        if (bit == 1 && n->right != NULL) { //if 1 go right
            n = n->right;
        }
    }
    if (i % BLOCK != 0
        && i != 0) { //After the loop write the remaining values in the buffer to the outfile
        write_bytes(outfile, buf, (i % BLOCK));
    }
    delete_tree(&root); //delete the tree
    if (stats) { //Print the stats if option selected
        double stat = 100 * (1 - ((double) total_bytes_read / (double) h.file_size));
        fprintf(stderr, "Compressed file size: %" PRId64 " bytes\n", total_bytes_read);
        fprintf(stderr, "Decompressed file size: %" PRId64 " bytes\n", h.file_size);
        fprintf(stderr, "Space saving: %0.2lf%c\n", stat, '%');
    }
    //Close the infile and the outfile
    close(infile);
    close(outfile);
    return 0;
}
