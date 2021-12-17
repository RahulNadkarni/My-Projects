Rahul Nadkarni

rnadkarni3@gmail.com

rnadkarn@ucsc.edu

Project Name: Steganography Project 

-----------
DESCRIPTION

In this project, the user first determines whether or not they want to encode a message or decode a message. If the user selects to encode a message, the user will then be prompted to choose the image that they would like their message encoded in. Then, the user selects either an existing file or a new image file for the message to be encoded in. Lastly, the user types in their desired message and determines the type of encoding they would like(Steganography Only, Steganography and Caesar Cipher, or Steganography and Huffman Codes). The Steganography only option results in a binary encoded output. These same options apply to the decoder as well. This project was done in CSE30 - Programming Abstractions in Python at UC Santa Cruz 

-----------
FILES

codec.py 

codec.py contains the implemenation for each type of coding. Each class represents a type of coding. The Codec class represents the default binary encoding and decoding option, the Caesar Cipher Class represents the caesar cipher implementation, and the Huffman class contains the implementation for huffman encoding and decoding.

steganography.py 

This file contains the implementations behind importing the message into the image. Using numpy arrays, the encoded message will be inputted into the image. The same applies for decoding as well. This function mainly serves as a method to receive encoded or decoded values and depending on which one, the program will either input the message into the image or output the decoded message.

main.py 

When running this program, run main.py as it is the driver file behind the project. It contains the main menus for both encoding and decoding and calls the different classes and functions appropriately. 

redbox.jpg

This is an image of a red square that can be used as a test case when running the program. 

-----------
INSTRUCTIONS

This program is intended to be run using any Python IDE. This specific program was developed using PyCharm.

--------
