#  Dictionaries and Word Frequency

#Description

From reading a list of words from an infile, the program will input these values into a binary search tree sorted in alphabetical order. The resultant outfile will contain an inOrder representation of the tree along with its numerical value, and a preOrder representation of the tree, but with just the keys.They all run within the specifications. Then, Taking in an infile of words, they are inserted as keys and the values are their frequencies. Count all the frequencies of the words in a file. 

# Files

Makefile, README.md, Dictionary.cpp, DictionaryTest.cpp, Order.cpp, Dictionary.h, WordFrequency.cpp

# Building the Program

In the command line, run make Order or make to create the Order executable. To make the  Word Frequency, run make WordFrequency. Make sure there is an infile and outfile present in the command line otherwise the program will segFault.

# Running the Program
This program takes in two files. The infile will contain all of the keys or the words, and the outfile will contain an inorder and preorder representation of the binary search tree for Order and the frequency of the words in WordFrequency.

# Bugs/Issues 

There are Memory Leaks 
