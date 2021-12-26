Huffman Coding in C
----------

DISCLAIMER:
----------
THIS IS A MODIFIED VERSION OF AN ASSIGNMENT DONE FOR CSE13S IN THE FALL 2021 QUARTER AT UC SANTA CRUZ. IF YOU ARE A CSE13S STUDENT DO NOT VIEW THE FOLLOWING SOURCE CODE!
---------
#Description
---------
This project runs a Huffman compression algorithm. A Huffman compression takes a file and compresses it down significantly yet still retains the same information. There are two main forms that a Huffman compression can take: an encoder or a decoder. The encoding algorithm reads in a file and builds a tree depending on the frequencies of each character. Once the tree is built, the program traverses through the tree and develops a code for each character in the tree. The code generated can be used by the decoder to understand the message. The decoder reads the code generated and deciphers its meaning. A zero would prompt the decoder to move traverse down the left of the tree and a one would cause the decoder to move to the right of the tree. By continuing this process until the end of the instructions, the message will be successfully deciphered.

Building the Program
-------------------
By running make all, the encode and decode executables will be made. If the user runs only make encode or only make decode, only encode or decode will be generated.

Running the Program
------------------
Run the program by selecting which test you would like to run. -i selects the infile, -o selects the outfile, -h prints the help menu, and -v prints out compression statistics. The same works with the decode file. 

Bugs/Issues
------------------
There are no prevalent issues in this program. However, when running Valgrind on the encoder, although there are no leaks, there is an error where it an opening is pointint to uninitialized bytes. This bug does not affect the encoding and decoding of the overall program.
