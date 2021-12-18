#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define OPTIONS "hvi:o:"
static inline Header set_header(int infile, int outfile);
void encoder(Node *root, int outfile);
void dump_header(int outfile, Header *h);
struct stat fd;
int main(int argc, char **argv) { //main
    bool stats = false;
    int infile = STDIN_FILENO; //set infile to standard in
    int outfile = STDOUT_FILENO; //set out to standard out
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS))
           != -1) { //read the command line and do the required functionalities
        switch (opt) {
        case 'h':
            printf(
                "SYNOPSIS\nA Huffman encoder.\nCompresses a file using the Huffman coding "
                "algorithm.\nUSAGE\n./encode [-h] [-i infile] [-o outfile]\nOPTIONS\n-h            "
                " Program usage and help.\n-v             Print compression statistics.\n-i infile "
                "     Input file to compress.\n-o outfile     Output of compressed data.");
            break;
        case 'i': infile = open(optarg, O_RDONLY); break;
        case 'o': outfile = open(optarg, O_WRONLY | O_APPEND | O_CREAT); break;
        case 'v': stats = true; break;
        default: abort(); return 1;
        }
    }
    //Make the histogram
    uint64_t histogram[ALPHABET] = { 0 };
    histogram[0] = 1; //increment first and last values by 1
    histogram[255] = 1;
    int filedesc = 0; //set the new file to either create or open
    uint8_t buf[BLOCK];
    int done = 0;
    char name[255];
    filedesc = open(name, O_RDWR | O_CREAT | O_EXCL | O_TRUNC, 0600);
    //read the file
    do {
        done = read_bytes(infile, buf, sizeof(buf));
        for (uint64_t i = 0; i < bytes_read; i++) {
            histogram[buf[i]] += 1;
        }
        if (infile == STDIN_FILENO && filedesc != 0) {
            write_bytes(filedesc, buf, bytes_read);
        }
    } while (done != 0);
    //build the tree and then build the codes based on the constructed tree
    Node *root = build_tree(histogram);
    Code table[ALPHABET] = { 0 };
    build_codes(root, table);
    int file;
    //reset the file to the start
    if (infile == STDIN_FILENO && filedesc != 0) {
        lseek(filedesc, 0, SEEK_SET);
        file = filedesc;
    } else {
        lseek(infile, 0, SEEK_SET);
        file = infile;
    }
    //create the header and set the magic number, permissions, tree size, and file size
    Header h = set_header(infile, outfile);
    dump_header(outfile, &h);
    encoder(root, outfile); //encode the message
    uint8_t buffer[BLOCK];
    int dones = 0; //read the file again until the end of the file
    do {
        dones = read_bytes(file, buffer, sizeof(buffer));
        for (uint64_t i = 0; i < bytes_read; i++) {
            Code code = table[buffer[i]];
            write_code(outfile, &code);
        }
        memset(buffer, 0, sizeof(buf)); //clear the buffer if done using for the next iteration
    } while (dones != 0);
    flush_codes(outfile); //print the codes to the outfile
    if (stats) { //if verbose option selected print following stats
        fstat(infile, &fd);
        double stat = 100 * (1 - (((double) bytes_written - h.file_size) / (double) h.file_size));
        fprintf(stderr, "Uncompressed file size: %" PRId64 " bytes\n", h.file_size);
        fprintf(stderr, "Compressed file size: %" PRId64 " bytes\n", bytes_written - h.file_size);
        fprintf(stderr, "Space saving: %0.2lf%c\n", stat, '%');
    }
    //Free Memory Used
    if (infile == STDIN_FILENO && filedesc != 0) {
        unlink(name);
        close(filedesc);
    }
    delete_tree(&root);
    close(infile);
    close(outfile);
    return 0;
}
//Function that sets header in the file
static inline Header set_header(int infile, int outfile) {
    Header h;
    fstat(infile, &fd);
    fchmod(outfile, fd.st_mode);
    h.magic = MAGIC;
    h.permissions = fd.st_mode;
    h.tree_size = (3 * symbols) - 1;
    h.file_size = fd.st_size;
    return h;
}
//Prints header out to outfile
void dump_header(int outfile, Header *h) {
    write_bytes(outfile, (uint8_t *) h, sizeof(Header));
}
void encoder(Node *root, int outfile) {
    if (root) { //if the root exists
        uint8_t leaf = 'L'; //Leaf = L
        uint8_t interior = 'I'; //Interior Node = I
        if (root->left == NULL && root->right == NULL) { //If reached a leaf node
            write_bytes(outfile, &leaf, 1); //Print L
            write_bytes(outfile, &root->symbol, 1); //Print the symbol which corresponds to the leaf
        } else { //If an Interior Node
            encoder(root->left, outfile); //Go left until hit
            encoder(root->right, outfile); //Go right until hit
            write_bytes(outfile, &interior, 1); //Print the I else
        }
    }
}
